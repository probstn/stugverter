#!/usr/bin/env python3
"""
Request a high-rate inverter UDP stream and print received packets plus sample rate.

Default target:
    TCP control: 10.10.210.166:3030
    UDP stream: 0.0.0.0:3040

Example:
    python3 scripts/teststream.py
    python3 scripts/teststream.py --address 0x2100 --address 0x2101 --address 0x2104
"""

from __future__ import annotations

import argparse
import socket
import struct
import time
from dataclasses import dataclass
from typing import Iterable

START = b"\xAA\x55"
TCP_HEADER_SIZE = 5
MAX_PAYLOAD = 250
DICT_RECORD_LEN = 25

CMD_DICTIONARY = 0x00
CMD_STREAM_START = 0x03
CMD_STREAM_STOP = 0x04

CMD_DICTIONARY_ACK = 0x80
CMD_STREAM_ACK = 0x83
CMD_STREAM_STOP_ACK = 0x84
CMD_ERROR = 0x8F

TYPE_UINT8 = 0
TYPE_INT8 = 1
TYPE_UINT16 = 2
TYPE_INT16 = 3
TYPE_UINT32 = 4
TYPE_INT32 = 5
TYPE_FLOAT32 = 6
TYPE_UINT64 = 7

TYPE_INFO = {
    TYPE_UINT8: (1, "<B"),
    TYPE_INT8: (1, "<b"),
    TYPE_UINT16: (2, "<H"),
    TYPE_INT16: (2, "<h"),
    TYPE_UINT32: (4, "<I"),
    TYPE_INT32: (4, "<i"),
    TYPE_FLOAT32: (4, "<f"),
    TYPE_UINT64: (8, "<Q"),
}


@dataclass(frozen=True)
class DictEntry:
    address: int
    type_code: int
    access: int
    group: int
    name: str
    unit: str

    @property
    def size(self) -> int:
        return TYPE_INFO[self.type_code][0]


@dataclass(frozen=True)
class Frame:
    cmd: int
    payload: bytes


class ProtocolError(RuntimeError):
    pass


class TcpClient:
    def __init__(self, host: str, port: int, timeout_s: float):
        self.sock = socket.create_connection((host, port), timeout=timeout_s)
        self.sock.settimeout(timeout_s)
        self.rx = bytearray()

    def close(self) -> None:
        self.sock.close()

    def send_frame(self, cmd: int, payload: bytes = b"") -> None:
        if len(payload) > MAX_PAYLOAD:
            raise ProtocolError(f"payload too large: {len(payload)}")
        frame = START + bytes([cmd]) + struct.pack("<H", len(payload)) + payload
        self.sock.sendall(frame)

    def read_frame(self, timeout_s: float) -> Frame:
        deadline = time.monotonic() + timeout_s
        while time.monotonic() < deadline:
            frame = self._pop_frame()
            if frame is not None:
                return frame

            remaining = max(0.0, deadline - time.monotonic())
            self.sock.settimeout(min(0.25, remaining))
            try:
                chunk = self.sock.recv(4096)
            except socket.timeout:
                continue

            if not chunk:
                raise ProtocolError("TCP connection closed")
            self.rx.extend(chunk)

        raise TimeoutError("timed out waiting for TCP response")

    def read_matching(self, wanted: Iterable[int], timeout_s: float) -> Frame:
        wanted_set = set(wanted)
        deadline = time.monotonic() + timeout_s
        while time.monotonic() < deadline:
            frame = self.read_frame(max(0.01, deadline - time.monotonic()))
            if frame.cmd == CMD_ERROR:
                raise ProtocolError(format_error(frame.payload))
            if frame.cmd in wanted_set:
                return frame
        raise TimeoutError(f"timed out waiting for {sorted(wanted_set)}")

    def _pop_frame(self) -> Frame | None:
        while len(self.rx) >= TCP_HEADER_SIZE:
            start = self.rx.find(START)
            if start < 0:
                del self.rx[:]
                return None
            if start > 0:
                del self.rx[:start]
                continue

            payload_len = self.rx[3] | (self.rx[4] << 8)
            if payload_len > MAX_PAYLOAD:
                del self.rx[0]
                continue

            frame_len = TCP_HEADER_SIZE + payload_len
            if len(self.rx) < frame_len:
                return None

            cmd = self.rx[2]
            payload = bytes(self.rx[TCP_HEADER_SIZE:frame_len])
            del self.rx[:frame_len]
            return Frame(cmd, payload)
        return None


def trim_ascii(blob: bytes) -> str:
    return blob.decode("ascii", errors="replace").strip()


def parse_dictionary_frame(payload: bytes) -> list[DictEntry]:
    if len(payload) % DICT_RECORD_LEN != 0:
        raise ProtocolError(f"bad dictionary payload length: {len(payload)}")

    entries: list[DictEntry] = []
    for offset in range(0, len(payload), DICT_RECORD_LEN):
        record = payload[offset : offset + DICT_RECORD_LEN]
        address = struct.unpack_from("<H", record, 0)[0]
        ctrl = record[2]
        type_code = ctrl & 0x0F
        access = (ctrl >> 4) & 0x03
        group = record[3]
        name = trim_ascii(record[4:20])
        unit = trim_ascii(record[20:25])
        if type_code in TYPE_INFO:
            entries.append(DictEntry(address, type_code, access, group, name, unit))
    return entries


def load_dictionary(client: TcpClient, timeout_s: float) -> list[DictEntry]:
    client.send_frame(CMD_DICTIONARY, b"\xFF")
    entries: list[DictEntry] = []
    deadline = time.monotonic() + timeout_s
    while time.monotonic() < deadline:
        try:
            frame = client.read_frame(max(0.01, deadline - time.monotonic()))
        except TimeoutError:
            break

        if frame.cmd == CMD_ERROR:
            raise ProtocolError(format_error(frame.payload))
        if frame.cmd == CMD_DICTIONARY_ACK:
            entries.extend(parse_dictionary_frame(frame.payload))

    if not entries:
        raise ProtocolError("dictionary request returned no entries")
    return sorted(entries, key=lambda e: e.address)


def parse_address(text: str) -> int:
    value = int(text, 0)
    if value < 0 or value > 0xFFFF:
        raise argparse.ArgumentTypeError("address must fit in uint16")
    return value


def choose_entries(dictionary: list[DictEntry], addresses: list[int], max_auto: int) -> list[DictEntry]:
    by_address = {entry.address: entry for entry in dictionary}
    if addresses:
        missing = [address for address in addresses if address not in by_address]
        if missing:
            pretty = ", ".join(f"0x{address:04X}" for address in missing)
            raise ProtocolError(f"address not in dictionary: {pretty}")
        return [by_address[address] for address in addresses]

    preferred_groups = {0x21, 0x22, 0x24, 0x25, 0x26, 0x27}
    telemetry = [entry for entry in dictionary if entry.group in preferred_groups]
    selected = telemetry[:max_auto] if telemetry else dictionary[:max_auto]
    if not selected:
        raise ProtocolError("no streamable dictionary entries found")
    return selected


def request_stream(client: TcpClient, stream_id: int, loop_divider: int, entries: list[DictEntry], timeout_s: float) -> list[DictEntry]:
    payload = bytes([stream_id & 0xFF, loop_divider & 0xFF])
    payload += b"".join(struct.pack("<H", entry.address) for entry in entries)
    client.send_frame(CMD_STREAM_START, payload)

    ack = client.read_matching({CMD_STREAM_ACK}, timeout_s)
    if len(ack.payload) < 2:
        raise ProtocolError("short stream ack")
    ack_id = ack.payload[0]
    count = ack.payload[1]
    if ack_id != stream_id:
        raise ProtocolError(f"stream ack id mismatch: expected {stream_id}, got {ack_id}")
    if len(ack.payload) != 2 + count * 2:
        raise ProtocolError("stream ack length mismatch")

    by_address = {entry.address: entry for entry in entries}
    ack_entries: list[DictEntry] = []
    for index in range(count):
        address = struct.unpack_from("<H", ack.payload, 2 + index * 2)[0]
        if address not in by_address:
            raise ProtocolError(f"stream ack returned unexpected address 0x{address:04X}")
        ack_entries.append(by_address[address])
    return ack_entries


def decode_value(blob: bytes, offset: int, entry: DictEntry) -> tuple[object, int]:
    size, fmt = TYPE_INFO[entry.type_code]
    if offset + size > len(blob):
        raise ProtocolError(f"datagram ended while decoding 0x{entry.address:04X}")
    return struct.unpack_from(fmt, blob, offset)[0], offset + size


def parse_udp_packet(packet: bytes, entries: list[DictEntry]) -> tuple[int, list[tuple[int, int, dict[str, object]]]]:
    if len(packet) < 4:
        raise ProtocolError(f"short UDP packet: {len(packet)}")
    if packet[:2] != START:
        raise ProtocolError(f"bad UDP start bytes: {packet[:2].hex()}")

    stream_id = packet[2]
    sample_count = packet[3]
    sample_size = sum(entry.size for entry in entries)
    expected = 4 + sample_count * (2 + 8 + sample_size)
    if len(packet) != expected:
        raise ProtocolError(f"UDP length mismatch: got {len(packet)}, expected {expected}, samples={sample_count}")

    offset = 4
    samples: list[tuple[int, int, dict[str, object]]] = []
    for _ in range(sample_count):
        seq = struct.unpack_from("<H", packet, offset)[0]
        offset += 2
        ticks = struct.unpack_from("<Q", packet, offset)[0]
        offset += 8

        decoded: dict[str, object] = {}
        for entry in entries:
            value, offset = decode_value(packet, offset, entry)
            decoded[entry.name or f"0x{entry.address:04X}"] = value
        samples.append((seq, ticks, decoded))

    return stream_id, samples


def format_error(payload: bytes) -> str:
    if len(payload) != 3:
        return f"device error with malformed payload: {payload.hex()}"
    address, code = struct.unpack_from("<HB", payload, 0)
    return f"device error addr=0x{address:04X} code=0x{code:02X}"


def main() -> int:
    parser = argparse.ArgumentParser(description="Request and print a high-rate inverter UDP stream")
    parser.add_argument("--host", default="10.10.210.166", help="inverter IPv4 address")
    parser.add_argument("--tcp-port", type=int, default=3030, help="TCP control port")
    parser.add_argument("--udp-port", type=int, default=3040, help="local UDP stream listen port")
    parser.add_argument("--stream-id", type=int, default=1, help="stream id")
    parser.add_argument("--divider", type=int, default=1, help="loop divider; 1 requests max rate")
    parser.add_argument("--address", action="append", type=parse_address, default=[], help="address to stream, repeatable")
    parser.add_argument("--auto-count", type=int, default=4, help="number of dictionary entries to auto-stream")
    parser.add_argument("--duration", type=float, default=0.0, help="seconds to run; 0 means until Ctrl-C")
    parser.add_argument("--tcp-timeout", type=float, default=3.0, help="TCP timeout seconds")
    parser.add_argument("--udp-timeout", type=float, default=2.0, help="UDP timeout seconds")
    parser.add_argument("--tick-hz", type=float, default=100000000.0, help="firmware timestamp tick rate")
    args = parser.parse_args()

    if args.stream_id < 0 or args.stream_id > 255:
        raise SystemExit("--stream-id must be in [0..255]")
    if args.divider < 1 or args.divider > 255:
        raise SystemExit("--divider must be in [1..255]")
    if args.auto_count < 1 or args.auto_count > 16:
        raise SystemExit("--auto-count must be in [1..16]")

    udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        udp.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    except OSError:
        pass
    udp.bind(("0.0.0.0", args.udp_port))
    udp.settimeout(args.udp_timeout)

    client = TcpClient(args.host, args.tcp_port, args.tcp_timeout)
    entries: list[DictEntry] = []
    stream_active = False

    try:
        dictionary = load_dictionary(client, args.tcp_timeout)
        entries = request_stream(
            client,
            args.stream_id,
            args.divider,
            choose_entries(dictionary, args.address, args.auto_count),
            args.tcp_timeout,
        )
        stream_active = True

        labels = ", ".join(f"0x{entry.address:04X}/{entry.name}" for entry in entries)
        print(f"stream id={args.stream_id} divider={args.divider} target={args.host}:{args.tcp_port} udp=0.0.0.0:{args.udp_port}")
        print(f"addresses: {labels}")

        packet_count = 0
        sample_count = 0
        first_wall = time.monotonic()
        last_wall = first_wall
        first_tick: int | None = None
        last_tick: int | None = None
        deadline = first_wall + args.duration if args.duration > 0 else None

        while deadline is None or time.monotonic() < deadline:
            try:
                packet, sender = udp.recvfrom(2048)
            except socket.timeout:
                print("UDP timeout waiting for stream packet")
                continue

            now = time.monotonic()
            try:
                stream_id, samples = parse_udp_packet(packet, entries)
            except ProtocolError as exc:
                print(f"bad UDP packet from {sender[0]}:{sender[1]}: {exc}")
                continue

            packet_count += 1
            sample_count += len(samples)
            seq_first = samples[0][0]
            seq_last = samples[-1][0]
            tick_first = samples[0][1]
            tick_last = samples[-1][1]
            if first_tick is None:
                first_tick = tick_first
            last_tick = tick_last

            wall_dt = max(now - first_wall, 1e-9)
            inst_dt = max(now - last_wall, 1e-9)
            avg_rate = sample_count / wall_dt
            inst_rate = len(samples) / inst_dt
            firmware_rate = 0.0
            if first_tick is not None and last_tick is not None and last_tick > first_tick:
                firmware_rate = sample_count / ((last_tick - first_tick) / args.tick_hz)

            values = samples[-1][2]
            values_text = ", ".join(f"{key}={value}" for key, value in values.items())
            print(
                f"packet={packet_count} from={sender[0]}:{sender[1]} bytes={len(packet)} "
                f"sid={stream_id} samples={len(samples)} seq={seq_first}->{seq_last} "
                f"ticks={tick_first}->{tick_last} rate={avg_rate:.1f}/s inst={inst_rate:.1f}/s "
                f"fw={firmware_rate:.1f}/s values: {values_text}"
            )
            last_wall = now

    except KeyboardInterrupt:
        print("\nstopping stream")
    finally:
        if stream_active:
            try:
                client.send_frame(CMD_STREAM_STOP, bytes([args.stream_id & 0xFF]))
                frame = client.read_matching({CMD_STREAM_STOP_ACK}, 0.5)
                print(f"stream stop ack: {frame.payload.hex()}")
            except Exception as exc:
                print(f"stream stop not confirmed: {exc}")
        client.close()
        udp.close()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
