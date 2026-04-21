#!/usr/bin/env python3
"""
Manual command-line client for the inverter Ethernet protocol.

Features:
- Dictionary request and pretty print
- Manual register read and write
- Config commit
- Stream start/stop/listen
- Built-in compliance-oriented test routine
- Detailed TX/RX hex debug logging
"""

from __future__ import annotations

import argparse
import dataclasses
import logging
import math
import socket
import struct
import sys
import time
from typing import Dict, List, Optional, Sequence, Tuple

START_BYTES = b"\xAA\x55"
MAX_PAYLOAD = 250
TCP_HEADER_SIZE = 5
DICT_RECORD_LEN = 24

CMD_DICTIONARY = 0x00
CMD_READ = 0x01
CMD_WRITE = 0x02
CMD_STREAM_START = 0x03
CMD_STREAM_STOP = 0x04
CMD_CONFIG_COMMIT = 0x06

CMD_DICTIONARY_ACK = 0x80
CMD_READ_ACK = 0x81
CMD_WRITE_ACK = 0x82
CMD_STREAM_ACK = 0x83
CMD_STREAM_STOP_ACK = 0x84
CMD_CONFIG_COMMIT_ACK = 0x86
CMD_ERROR = 0x8F

ERR_INVALID_ADDR = 0x01
ERR_ACCESS_DENIED = 0x02
ERR_OTHER = 0x03

ACCESS_READ_ONLY = 0x00
ACCESS_WRITE_AFTER_RESTART = 0x01
ACCESS_WRITE_LIVE = 0x02

TYPE_UINT8 = 0
TYPE_INT8 = 1
TYPE_UINT16 = 2
TYPE_INT16 = 3
TYPE_UINT32 = 4
TYPE_INT32 = 5
TYPE_FLOAT32 = 6
TYPE_UINT64 = 7

CMD_NAMES = {
    CMD_DICTIONARY: "Dict-Req",
    CMD_READ: "Read-Req",
    CMD_WRITE: "Write-Req",
    CMD_STREAM_START: "Stream-Req",
    CMD_STREAM_STOP: "Stream-Stop",
    CMD_CONFIG_COMMIT: "Update-Config",
    CMD_DICTIONARY_ACK: "Dict-Data",
    CMD_READ_ACK: "Read-Data",
    CMD_WRITE_ACK: "Write-Ack",
    CMD_STREAM_ACK: "Stream-Ack",
    CMD_STREAM_STOP_ACK: "Stream-Stop-Ack",
    CMD_CONFIG_COMMIT_ACK: "Update-Config-Ack",
    CMD_ERROR: "Error-Resp",
}

ERR_NAMES = {
    ERR_INVALID_ADDR: "Invalid Address",
    ERR_ACCESS_DENIED: "Access Denied",
    ERR_OTHER: "Other / malformed request",
}

ACCESS_NAMES = {
    ACCESS_READ_ONLY: "read-only",
    ACCESS_WRITE_AFTER_RESTART: "write-after-restart",
    ACCESS_WRITE_LIVE: "write-live",
}

TYPE_INFO = {
    TYPE_UINT8: (1, "<B", "uint8"),
    TYPE_INT8: (1, "<b", "int8"),
    TYPE_UINT16: (2, "<H", "uint16"),
    TYPE_INT16: (2, "<h", "int16"),
    TYPE_UINT32: (4, "<I", "uint32"),
    TYPE_INT32: (4, "<i", "int32"),
    TYPE_FLOAT32: (4, "<f", "float32"),
    TYPE_UINT64: (8, "<Q", "uint64"),
}

TYPE_NAME_TO_CODE = {v[2]: k for k, v in TYPE_INFO.items()}


def hex_bytes(data: bytes) -> str:
    return " ".join(f"{b:02X}" for b in data)


def now_s() -> float:
    return time.monotonic()


def parse_int(text: str) -> int:
    return int(text, 0)


@dataclasses.dataclass
class Frame:
    cmd: int
    payload: bytes
    raw: bytes


@dataclasses.dataclass
class DictEntry:
    address: int
    type_code: int
    access: int
    name: str
    unit: str

    @property
    def size(self) -> int:
        return TYPE_INFO.get(self.type_code, (0, "", "unknown"))[0]

    @property
    def type_name(self) -> str:
        return TYPE_INFO.get(self.type_code, (0, "", f"unknown({self.type_code})"))[2]


class ProtocolError(Exception):
    pass


class ProtocolClient:
    def __init__(self, host: str, tcp_port: int, timeout_s: float, logger: logging.Logger):
        self.host = host
        self.tcp_port = tcp_port
        self.timeout_s = timeout_s
        self.logger = logger

        self.sock: Optional[socket.socket] = None
        self.rx_buf = bytearray()
        self.pending_frames: List[Frame] = []

    def connect(self) -> None:
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(self.timeout_s)
        self.sock.connect((self.host, self.tcp_port))
        self.logger.info("TCP connected to %s:%d", self.host, self.tcp_port)

    def close(self) -> None:
        if self.sock is not None:
            try:
                self.sock.close()
            finally:
                self.sock = None
        self.logger.info("TCP closed")

    def send_frame(self, cmd: int, payload: bytes = b"") -> None:
        if self.sock is None:
            raise ProtocolError("TCP socket not connected")
        if len(payload) > MAX_PAYLOAD:
            raise ProtocolError(f"payload too large: {len(payload)}")

        frame = START_BYTES + bytes([cmd]) + struct.pack("<H", len(payload)) + payload
        self.logger.debug("TX CMD=0x%02X (%s) LEN=%d", cmd, CMD_NAMES.get(cmd, "Unknown"), len(payload))
        self.logger.debug("TX RAW: %s", hex_bytes(frame))
        self.sock.sendall(frame)

    def _read_from_socket(self, timeout_s: float) -> bool:
        if self.sock is None:
            raise ProtocolError("TCP socket not connected")

        self.sock.settimeout(timeout_s)
        try:
            data = self.sock.recv(2048)
        except socket.timeout:
            return False

        if not data:
            raise ProtocolError("TCP connection closed by peer")

        self.logger.debug("RX CHUNK (%d): %s", len(data), hex_bytes(data))
        self.rx_buf.extend(data)
        return True

    def _extract_frames(self) -> List[Frame]:
        out: List[Frame] = []

        while len(self.rx_buf) >= TCP_HEADER_SIZE:
            if self.rx_buf[0] != START_BYTES[0] or self.rx_buf[1] != START_BYTES[1]:
                dropped = self.rx_buf[0]
                del self.rx_buf[0]
                self.logger.warning("RX resync dropped byte: 0x%02X", dropped)
                continue

            cmd = self.rx_buf[2]
            payload_len = self.rx_buf[3] | (self.rx_buf[4] << 8)
            if payload_len > MAX_PAYLOAD:
                self.logger.warning("RX invalid payload len=%d, dropping one byte for resync", payload_len)
                del self.rx_buf[0]
                continue

            frame_len = TCP_HEADER_SIZE + payload_len
            if len(self.rx_buf) < frame_len:
                break

            raw = bytes(self.rx_buf[:frame_len])
            payload = bytes(self.rx_buf[TCP_HEADER_SIZE:frame_len])
            del self.rx_buf[:frame_len]

            self.logger.debug(
                "RX FRAME CMD=0x%02X (%s) LEN=%d RAW=%s",
                cmd,
                CMD_NAMES.get(cmd, "Unknown"),
                payload_len,
                hex_bytes(raw),
            )
            out.append(Frame(cmd=cmd, payload=payload, raw=raw))

        return out

    def recv_frame(self, timeout_s: float) -> Frame:
        deadline = now_s() + timeout_s

        while now_s() < deadline:
            if self.pending_frames:
                return self.pending_frames.pop(0)

            frames = self._extract_frames()
            if frames:
                self.pending_frames.extend(frames)
                return self.pending_frames.pop(0)

            remaining = max(0.01, deadline - now_s())
            self._read_from_socket(min(0.2, remaining))

        raise TimeoutError(f"timeout waiting for frame ({timeout_s:.2f}s)")

    def recv_frames_until_silence(self, max_total_s: float, silence_s: float) -> List[Frame]:
        collected: List[Frame] = []
        end_time = now_s() + max_total_s
        last_rx = now_s()

        if self.pending_frames:
            collected.extend(self.pending_frames)
            self.pending_frames.clear()
            last_rx = now_s()

        while now_s() < end_time:
            frames = self._extract_frames()
            if frames:
                collected.extend(frames)
                last_rx = now_s()
                continue

            if collected and (now_s() - last_rx) >= silence_s:
                break

            remaining = max(0.0, end_time - now_s())
            if remaining <= 0.0:
                break

            got = self._read_from_socket(min(0.15, remaining))
            if got:
                last_rx = now_s()

        return collected


class InverterCli:
    def __init__(self, args: argparse.Namespace, logger: logging.Logger):
        self.args = args
        self.logger = logger
        self.client = ProtocolClient(args.host, args.tcp_port, args.tcp_timeout, logger)

    def run(self) -> int:
        self.client.connect()
        try:
            cmd = self.args.command
            if cmd == "dict":
                return self.cmd_dict()
            if cmd == "read":
                return self.cmd_read()
            if cmd == "write":
                return self.cmd_write()
            if cmd == "commit":
                return self.cmd_commit()
            if cmd == "stream-start":
                return self.cmd_stream_start()
            if cmd == "stream-stop":
                return self.cmd_stream_stop()
            if cmd == "stream-listen":
                return self.cmd_stream_listen()
            if cmd == "test-everything":
                return self.cmd_test_everything()

            raise ProtocolError(f"unsupported command: {cmd}")
        finally:
            self.client.close()

    def _expect_frame(self, expected_cmds: Tuple[int, ...], timeout_s: float) -> Frame:
        while True:
            frame = self.client.recv_frame(timeout_s)
            if frame.cmd == CMD_ERROR:
                self._raise_error(frame)
            if frame.cmd in expected_cmds:
                return frame

            self.logger.warning(
                "Ignored unexpected frame CMD=0x%02X (%s)",
                frame.cmd,
                CMD_NAMES.get(frame.cmd, "Unknown"),
            )

    def _expect_error(self, timeout_s: float) -> Tuple[int, int]:
        frame = self.client.recv_frame(timeout_s)
        if frame.cmd != CMD_ERROR:
            raise ProtocolError(
                f"expected Error-Resp, got CMD=0x{frame.cmd:02X} ({CMD_NAMES.get(frame.cmd, 'Unknown')})"
            )

        if len(frame.payload) != 3:
            raise ProtocolError(f"malformed Error-Resp payload length: {len(frame.payload)}")

        address = frame.payload[0] | (frame.payload[1] << 8)
        code = frame.payload[2]
        self.logger.info(
            "Error-Resp: address=0x%04X code=0x%02X (%s)",
            address,
            code,
            ERR_NAMES.get(code, "Unknown"),
        )
        return address, code

    @staticmethod
    def _raise_error(frame: Frame) -> None:
        if len(frame.payload) == 3:
            address = frame.payload[0] | (frame.payload[1] << 8)
            code = frame.payload[2]
            raise ProtocolError(
                f"device Error-Resp address=0x{address:04X} code=0x{code:02X} ({ERR_NAMES.get(code, 'Unknown')})"
            )
        raise ProtocolError(f"malformed Error-Resp payload length: {len(frame.payload)}")

    def fetch_dictionary(self) -> Dict[int, DictEntry]:
        self.client.send_frame(CMD_DICTIONARY, b"")
        frames = self.client.recv_frames_until_silence(self.args.dict_total_timeout, self.args.dict_silence_timeout)
        dict_frames = [f for f in frames if f.cmd == CMD_DICTIONARY_ACK]
        err_frames = [f for f in frames if f.cmd == CMD_ERROR]

        if err_frames:
            self._raise_error(err_frames[0])
        if not dict_frames:
            raise ProtocolError("no dictionary data received")

        out: Dict[int, DictEntry] = {}
        for frame_idx, frame in enumerate(dict_frames):
            if (len(frame.payload) % DICT_RECORD_LEN) != 0:
                raise ProtocolError(
                    f"dictionary frame {frame_idx} length {len(frame.payload)} is not multiple of {DICT_RECORD_LEN}"
                )

            for i in range(0, len(frame.payload), DICT_RECORD_LEN):
                rec = frame.payload[i: i + DICT_RECORD_LEN]
                address = rec[0] | (rec[1] << 8)
                ctrl = rec[2]
                type_code = ctrl & 0x0F
                access = (ctrl >> 4) & 0x03
                name = rec[3:19].decode("ascii", errors="replace").rstrip(" ")
                unit = rec[19:24].decode("ascii", errors="replace").rstrip(" ")

                out[address] = DictEntry(
                    address=address,
                    type_code=type_code,
                    access=access,
                    name=name,
                    unit=unit,
                )

        return out

    @staticmethod
    def decode_typed(entry: DictEntry, raw: bytes):
        if entry.type_code not in TYPE_INFO:
            raise ProtocolError(f"unknown type code {entry.type_code} at 0x{entry.address:04X}")
        size, fmt, _ = TYPE_INFO[entry.type_code]
        if len(raw) != size:
            raise ProtocolError(
                f"decode size mismatch for 0x{entry.address:04X}: expected {size}, got {len(raw)}"
            )
        return struct.unpack(fmt, raw)[0]

    @staticmethod
    def encode_typed(type_code: int, value_text: str) -> bytes:
        if type_code not in TYPE_INFO:
            raise ProtocolError(f"unknown type code {type_code}")
        _, fmt, tname = TYPE_INFO[type_code]

        if tname == "float32":
            value = float(value_text)
            if not math.isfinite(value):
                raise ProtocolError("float value must be finite")
            return struct.pack(fmt, value)

        value = parse_int(value_text)
        try:
            return struct.pack(fmt, value)
        except struct.error as exc:
            raise ProtocolError(f"value '{value_text}' out of range for {tname}") from exc

    def read_values(self, addresses: Sequence[int], timeout_s: float, dct: Dict[int, DictEntry]) -> Dict[int, bytes]:
        if not addresses:
            raise ProtocolError("no addresses provided")

        payload = b"".join(struct.pack("<H", a) for a in addresses)
        self.client.send_frame(CMD_READ, payload)
        frame = self._expect_frame((CMD_READ_ACK,), timeout_s)

        expected_len = 0
        for addr in addresses:
            if addr not in dct:
                raise ProtocolError(f"address 0x{addr:04X} not found in dictionary")
            expected_len += dct[addr].size

        if len(frame.payload) != expected_len:
            raise ProtocolError(
                f"Read-Data payload length mismatch: expected {expected_len}, got {len(frame.payload)}"
            )

        out: Dict[int, bytes] = {}
        idx = 0
        for addr in addresses:
            size = dct[addr].size
            out[addr] = frame.payload[idx: idx + size]
            idx += size

        return out

    def cmd_dict(self) -> int:
        dct = self.fetch_dictionary()
        entries = sorted(dct.values(), key=lambda e: e.address)

        print(f"dictionary entries: {len(entries)}")
        print("address  type      access               name             unit")
        print("-------  --------  -------------------  ---------------  -----")
        for e in entries:
            print(
                f"0x{e.address:04X}   {e.type_name:<8}  {ACCESS_NAMES.get(e.access, 'unknown'):<19}  "
                f"{e.name:<15}  {e.unit}"
            )

        return 0

    def cmd_read(self) -> int:
        addresses = [parse_int(a) & 0xFFFF for a in self.args.addr]
        dct = self.fetch_dictionary()
        raw = self.read_values(addresses, self.args.tcp_timeout, dct)

        print("read result:")
        for addr in addresses:
            entry = dct[addr]
            decoded = self.decode_typed(entry, raw[addr])
            print(
                f"0x{addr:04X} {entry.name} ({entry.type_name}, {ACCESS_NAMES.get(entry.access, 'unknown')}): "
                f"{decoded}  raw={hex_bytes(raw[addr])}"
            )
        return 0

    def cmd_write(self) -> int:
        addr = parse_int(self.args.addr) & 0xFFFF
        dct = self.fetch_dictionary()

        if addr not in dct:
            raise ProtocolError(f"address 0x{addr:04X} not found in dictionary")
        entry = dct[addr]

        if self.args.type is not None:
            if self.args.type not in TYPE_NAME_TO_CODE:
                raise ProtocolError(f"unsupported --type '{self.args.type}'")
            type_code = TYPE_NAME_TO_CODE[self.args.type]
            if type_code != entry.type_code:
                raise ProtocolError(
                    f"type mismatch: dictionary says {entry.type_name}, but --type is {self.args.type}"
                )

        raw_value = self.encode_typed(entry.type_code, self.args.value)
        payload = struct.pack("<H", addr) + raw_value
        self.client.send_frame(CMD_WRITE, payload)

        ack = self._expect_frame((CMD_WRITE_ACK,), self.args.tcp_timeout)
        if len(ack.payload) != 3:
            raise ProtocolError(f"Write-Ack payload length must be 3, got {len(ack.payload)}")

        first_addr = ack.payload[0] | (ack.payload[1] << 8)
        count = ack.payload[2]
        if first_addr != addr or count != 1:
            raise ProtocolError(
                f"unexpected Write-Ack: first=0x{first_addr:04X}, count={count}, expected first=0x{addr:04X}, count=1"
            )

        print(
            f"write ok: 0x{addr:04X} {entry.name} ({entry.type_name}) = {self.args.value} "
            f"raw={hex_bytes(raw_value)}"
        )

        if self.args.verify:
            verify = self.read_values([addr], self.args.tcp_timeout, dct)[addr]
            decoded = self.decode_typed(entry, verify)
            print(f"verify readback: {decoded} raw={hex_bytes(verify)}")

        if self.args.commit:
            self.client.send_frame(CMD_CONFIG_COMMIT, b"")
            commit_ack = self._expect_frame((CMD_CONFIG_COMMIT_ACK,), self.args.tcp_timeout)
            if len(commit_ack.payload) != 0:
                raise ProtocolError(f"Update-Config-Ack payload must be empty, got {len(commit_ack.payload)}")
            print("config commit ack received")

        return 0

    def cmd_commit(self) -> int:
        self.client.send_frame(CMD_CONFIG_COMMIT, b"")
        ack = self._expect_frame((CMD_CONFIG_COMMIT_ACK,), self.args.tcp_timeout)
        if len(ack.payload) != 0:
            raise ProtocolError(f"Update-Config-Ack payload must be empty, got {len(ack.payload)}")

        print("commit ok")
        return 0

    def cmd_stream_start(self) -> int:
        stream_id = self.args.stream_id
        freq_x100 = self.args.freq_x100
        addrs = [parse_int(a) & 0xFFFF for a in self.args.addr]

        if not addrs:
            raise ProtocolError("stream-start requires at least one address")

        payload = bytes([stream_id]) + struct.pack("<H", freq_x100) + b"".join(struct.pack("<H", a) for a in addrs)
        self.client.send_frame(CMD_STREAM_START, payload)

        ack = self._expect_frame((CMD_STREAM_ACK,), self.args.tcp_timeout)
        if len(ack.payload) < 2:
            raise ProtocolError("Stream-Ack payload too short")

        ack_id = ack.payload[0]
        count = ack.payload[1]
        addr_bytes = ack.payload[2:]
        if ack_id != stream_id:
            raise ProtocolError(f"stream id mismatch in ack: expected {stream_id}, got {ack_id}")
        if len(addr_bytes) != count * 2:
            raise ProtocolError(
                f"Stream-Ack address section length mismatch: expected {count * 2}, got {len(addr_bytes)}"
            )

        ack_addrs = [addr_bytes[i] | (addr_bytes[i + 1] << 8) for i in range(0, len(addr_bytes), 2)]

        print(f"stream start ok: id={ack_id}, count={count}, addresses={[f'0x{a:04X}' for a in ack_addrs]}")
        return 0

    def cmd_stream_stop(self) -> int:
        stream_id = self.args.stream_id
        self.client.send_frame(CMD_STREAM_STOP, bytes([stream_id]))

        if self.args.allow_inactive_error:
            frame = self.client.recv_frame(self.args.tcp_timeout)
            if frame.cmd == CMD_STREAM_STOP_ACK:
                if len(frame.payload) != 1 or frame.payload[0] != stream_id:
                    raise ProtocolError("invalid Stream-Stop-Ack payload")
                print(f"stream stop ack: id={stream_id}")
                return 0

            if frame.cmd == CMD_ERROR:
                address, code = self._expect_error_from_frame(frame)
                print(f"stream stop returned error: address=0x{address:04X} code=0x{code:02X} ({ERR_NAMES.get(code, 'Unknown')})")
                return 0

            raise ProtocolError(f"unexpected response CMD=0x{frame.cmd:02X}")

        ack = self._expect_frame((CMD_STREAM_STOP_ACK,), self.args.tcp_timeout)
        if len(ack.payload) != 1 or ack.payload[0] != stream_id:
            raise ProtocolError("invalid Stream-Stop-Ack payload")

        print(f"stream stop ack: id={stream_id}")
        return 0

    @staticmethod
    def _expect_error_from_frame(frame: Frame) -> Tuple[int, int]:
        if frame.cmd != CMD_ERROR:
            raise ProtocolError("frame is not Error-Resp")
        if len(frame.payload) != 3:
            raise ProtocolError("malformed Error-Resp")

        address = frame.payload[0] | (frame.payload[1] << 8)
        code = frame.payload[2]
        return address, code

    def cmd_stream_listen(self) -> int:
        stream_id = self.args.stream_id
        packet_count = self.args.packets

        dct = self.fetch_dictionary() if self.args.decode else {}
        decode_addrs: List[int] = [parse_int(a) & 0xFFFF for a in self.args.addr] if self.args.addr else []
        decode_entries: List[DictEntry] = [dct[a] for a in decode_addrs] if decode_addrs else []

        udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        udp.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            udp.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        except OSError:
            pass

        udp.bind(("", self.args.udp_port))
        udp.settimeout(self.args.udp_timeout)

        print(f"listening UDP on 0.0.0.0:{self.args.udp_port}, expecting stream id {stream_id}, packets={packet_count}")

        received = 0
        deadline = now_s() + self.args.listen_timeout

        try:
            while received < packet_count and now_s() < deadline:
                try:
                    pkt, sender = udp.recvfrom(2048)
                except socket.timeout:
                    continue

                self.logger.debug("UDP RX from %s:%d len=%d raw=%s", sender[0], sender[1], len(pkt), hex_bytes(pkt))

                if len(pkt) < 14:
                    print(f"ignored short UDP packet len={len(pkt)}")
                    continue
                if pkt[0:2] != START_BYTES:
                    print("ignored UDP packet without AA55 start")
                    continue

                sid = pkt[2]
                seq = pkt[3] | (pkt[4] << 8)
                ts = int.from_bytes(pkt[5:13], "little", signed=False)
                count = pkt[13]
                data = pkt[14:]

                if sid != stream_id:
                    print(f"ignored packet for stream id {sid}")
                    continue

                received += 1
                print(f"packet {received}/{packet_count}: sid={sid} seq={seq} ts={ts} count={count} data_len={len(data)}")

                if decode_entries:
                    expected_data_len = sum(e.size for e in decode_entries)
                    if expected_data_len != len(data):
                        print(
                            f"  decode skipped, expected data_len {expected_data_len} from --addr list but got {len(data)}"
                        )
                        continue

                    idx = 0
                    for e in decode_entries:
                        raw = data[idx: idx + e.size]
                        val = self.decode_typed(e, raw)
                        print(
                            f"  0x{e.address:04X} {e.name} ({e.type_name}) = {val} raw={hex_bytes(raw)}"
                        )
                        idx += e.size

            if received < packet_count:
                raise ProtocolError(f"timeout waiting UDP packets: got {received}/{packet_count}")

        finally:
            udp.close()

        print("stream listen complete")
        return 0

    def cmd_test_everything(self) -> int:
        start = now_s()
        print("running compliance-oriented end-to-end checks...")

        dct = self.fetch_dictionary()
        if not dct:
            raise ProtocolError("dictionary empty")
        print(f"[PASS] dictionary parsed ({len(dct)} entries)")

        self.client.send_frame(CMD_DICTIONARY, b"\x00")
        _, code = self._expect_error(self.args.tcp_timeout)
        if code != ERR_OTHER:
            raise ProtocolError(f"dictionary non-empty payload expected error 0x03, got 0x{code:02X}")
        print("[PASS] dictionary rejects non-empty payload")

        first_addr = min(dct.keys())
        read_data = self.read_values([first_addr], self.args.tcp_timeout, dct)
        value = self.decode_typed(dct[first_addr], read_data[first_addr])
        print(f"[PASS] read single register 0x{first_addr:04X} = {value}")

        ro_entry = next((e for e in dct.values() if e.access == ACCESS_READ_ONLY), None)
        if ro_entry is not None:
            fake = bytes([0] * ro_entry.size)
            self.client.send_frame(CMD_WRITE, struct.pack("<H", ro_entry.address) + fake)
            addr, code = self._expect_error(self.args.tcp_timeout)
            if code != ERR_ACCESS_DENIED or addr != ro_entry.address:
                raise ProtocolError("read-only write did not return expected access denied")
            print("[PASS] write denied on read-only parameter")

        live_entry = next((e for e in dct.values() if e.access == ACCESS_WRITE_LIVE), None)
        if live_entry is not None:
            before_raw = self.read_values([live_entry.address], self.args.tcp_timeout, dct)[live_entry.address]
            before_val = self.decode_typed(live_entry, before_raw)
            mutated_raw = self._mutate_for_test(live_entry, before_val)

            self.client.send_frame(CMD_WRITE, struct.pack("<H", live_entry.address) + mutated_raw)
            ack = self._expect_frame((CMD_WRITE_ACK,), self.args.tcp_timeout)
            if len(ack.payload) != 3 or (ack.payload[0] | (ack.payload[1] << 8)) != live_entry.address or ack.payload[2] != 1:
                raise ProtocolError("write-live ack mismatch")

            after_raw = self.read_values([live_entry.address], self.args.tcp_timeout, dct)[live_entry.address]
            if after_raw != mutated_raw:
                raise ProtocolError("write-live verification mismatch")

            self.client.send_frame(CMD_WRITE, struct.pack("<H", live_entry.address) + before_raw)
            _ = self._expect_frame((CMD_WRITE_ACK,), self.args.tcp_timeout)
            print("[PASS] write-live roundtrip and restore")

        self.client.send_frame(CMD_CONFIG_COMMIT, b"")
        ack = self._expect_frame((CMD_CONFIG_COMMIT_ACK,), self.args.tcp_timeout)
        if len(ack.payload) != 0:
            raise ProtocolError("config commit ack payload must be empty")
        print("[PASS] config commit empty payload/ack")

        self.client.send_frame(CMD_CONFIG_COMMIT, b"\x01")
        _, code = self._expect_error(self.args.tcp_timeout)
        if code != ERR_OTHER:
            raise ProtocolError(f"config commit non-empty payload expected error 0x03, got 0x{code:02X}")
        print("[PASS] config commit rejects non-empty payload")

        stream_addrs = [a for a in sorted(dct.keys())[:3]]
        stream_id = self.args.stream_id
        freq_x100 = self.args.freq_x100

        payload = bytes([stream_id]) + struct.pack("<H", freq_x100) + b"".join(struct.pack("<H", a) for a in stream_addrs)
        self.client.send_frame(CMD_STREAM_START, payload)
        stream_ack = self._expect_frame((CMD_STREAM_ACK,), self.args.tcp_timeout)
        if len(stream_ack.payload) < 2 or stream_ack.payload[0] != stream_id:
            raise ProtocolError("stream ack mismatch")
        print("[PASS] stream start ack")

        udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        udp.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            udp.bind(("", self.args.udp_port))
            udp.settimeout(self.args.udp_timeout)

            got_udp = False
            deadline = now_s() + self.args.listen_timeout
            while now_s() < deadline:
                try:
                    pkt, sender = udp.recvfrom(2048)
                except socket.timeout:
                    continue

                self.logger.debug("UDP RX from %s:%d len=%d raw=%s", sender[0], sender[1], len(pkt), hex_bytes(pkt))
                if len(pkt) >= 14 and pkt[0:2] == START_BYTES and pkt[2] == stream_id:
                    got_udp = True
                    break

            if not got_udp:
                raise ProtocolError("did not receive UDP stream packet for active stream")
        finally:
            udp.close()

        print("[PASS] UDP stream packet received")

        self.client.send_frame(CMD_STREAM_STOP, bytes([stream_id]))
        stop_ack = self._expect_frame((CMD_STREAM_STOP_ACK,), self.args.tcp_timeout)
        if len(stop_ack.payload) != 1 or stop_ack.payload[0] != stream_id:
            raise ProtocolError("stream stop ack mismatch")
        print("[PASS] stream stop ack")

        self.client.send_frame(CMD_STREAM_STOP, bytes([stream_id]))
        _, code = self._expect_error(self.args.tcp_timeout)
        if code != ERR_OTHER:
            raise ProtocolError("inactive stream stop did not return expected error code 0x03")
        print("[PASS] inactive stream stop error")

        elapsed = now_s() - start
        print(f"all checks passed in {elapsed:.2f}s")
        return 0

    @staticmethod
    def _mutate_for_test(entry: DictEntry, old_value) -> bytes:
        t = entry.type_code
        if t == TYPE_FLOAT32:
            new_val = old_value + 0.5 if isinstance(old_value, float) and math.isfinite(old_value) else 1.0
            return struct.pack("<f", float(new_val))
        if t == TYPE_UINT8:
            return struct.pack("<B", (int(old_value) + 1) & 0xFF)
        if t == TYPE_INT8:
            v = int(old_value)
            return struct.pack("<b", (v + 1) if v < 127 else (v - 1))
        if t == TYPE_UINT16:
            return struct.pack("<H", (int(old_value) + 1) & 0xFFFF)
        if t == TYPE_INT16:
            v = int(old_value)
            return struct.pack("<h", (v + 1) if v < 32767 else (v - 1))
        if t == TYPE_UINT32:
            return struct.pack("<I", (int(old_value) + 1) & 0xFFFFFFFF)
        if t == TYPE_INT32:
            v = int(old_value)
            return struct.pack("<i", (v + 1) if v < 2147483647 else (v - 1))
        if t == TYPE_UINT64:
            return struct.pack("<Q", (int(old_value) + 1) & 0xFFFFFFFFFFFFFFFF)

        raise ProtocolError(f"unsupported type for mutation: {t}")


def setup_logger(log_file: str, verbose: bool) -> logging.Logger:
    logger = logging.getLogger("inverter-cli")
    logger.handlers.clear()
    logger.setLevel(logging.DEBUG)

    fmt = logging.Formatter("%(asctime)s | %(levelname)-7s | %(message)s")

    console = logging.StreamHandler(sys.stdout)
    console.setLevel(logging.DEBUG if verbose else logging.INFO)
    console.setFormatter(fmt)

    file_handler = logging.FileHandler(log_file, mode="w", encoding="utf-8")
    file_handler.setLevel(logging.DEBUG)
    file_handler.setFormatter(fmt)

    logger.addHandler(console)
    logger.addHandler(file_handler)
    return logger


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Manual inverter Ethernet protocol CLI",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )

    parser.add_argument("--host", required=True, help="Inverter IPv4 address")
    parser.add_argument("--tcp-port", type=int, default=3030, help="TCP control port")
    parser.add_argument("--udp-port", type=int, default=3040, help="UDP stream listen port")
    parser.add_argument("--tcp-timeout", type=float, default=2.0, help="TCP response timeout in seconds")
    parser.add_argument("--udp-timeout", type=float, default=0.5, help="UDP receive timeout in seconds")
    parser.add_argument("--listen-timeout", type=float, default=5.0, help="Max listen time for stream packets")
    parser.add_argument("--dict-total-timeout", type=float, default=3.0, help="Max dictionary receive time")
    parser.add_argument("--dict-silence-timeout", type=float, default=0.3, help="Dictionary silence completion timeout")
    parser.add_argument("--freq-x100", type=int, default=1000, help="Default stream frequency (x100 Hz)")
    parser.add_argument("--stream-id", type=int, default=1, help="Default stream ID")
    parser.add_argument("--log-file", default="inverter_cli.log", help="Detailed log file path")
    parser.add_argument("--verbose", action="store_true", help="Enable verbose console debug output")

    sub = parser.add_subparsers(dest="command", required=True)

    sub.add_parser("dict", help="Request and print register dictionary")

    p_read = sub.add_parser("read", help="Read one or more registers")
    p_read.add_argument("--addr", nargs="+", required=True, help="Register addresses (e.g. 0x1301 0x2100)")

    p_write = sub.add_parser("write", help="Write one register")
    p_write.add_argument("--addr", required=True, help="Register address (e.g. 0x1301)")
    p_write.add_argument("--value", required=True, help="Value text (int supports 0x.., float decimal)")
    p_write.add_argument(
        "--type",
        choices=sorted(TYPE_NAME_TO_CODE.keys()),
        help="Optional explicit type, must match dictionary",
    )
    p_write.add_argument("--verify", action="store_true", help="Read back after write")
    p_write.add_argument("--commit", action="store_true", help="Send Update-Config after write")

    sub.add_parser("commit", help="Send Update-Config command")

    p_sstart = sub.add_parser("stream-start", help="Start stream")
    p_sstart.add_argument("--addr", nargs="+", required=True, help="Stream register addresses")

    p_sstop = sub.add_parser("stream-stop", help="Stop stream")
    p_sstop.add_argument("--allow-inactive-error", action="store_true", help="Treat Error-Resp as non-fatal output")

    p_slisten = sub.add_parser("stream-listen", help="Listen for UDP stream packets")
    p_slisten.add_argument("--packets", type=int, default=5, help="Number of packets to capture")
    p_slisten.add_argument("--decode", action="store_true", help="Fetch dictionary and decode packet data")
    p_slisten.add_argument("--addr", nargs="*", help="Address order for decode (must match Stream-Ack order)")

    sub.add_parser("test-everything", help="Run compliance-oriented end-to-end command checks")

    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    if args.stream_id < 0 or args.stream_id > 255:
        print("--stream-id must be in range [0..255]", file=sys.stderr)
        return 2
    if args.freq_x100 <= 0:
        print("--freq-x100 must be > 0", file=sys.stderr)
        return 2

    logger = setup_logger(args.log_file, args.verbose)

    try:
        cli = InverterCli(args, logger)
        return cli.run()
    except Exception as exc:  # pylint: disable=broad-except
        logger.error("Command failed: %s: %s", type(exc).__name__, exc)
        return 1


if __name__ == "__main__":
    sys.exit(main())
