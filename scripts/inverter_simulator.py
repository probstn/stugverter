#!/usr/bin/env python3
"""
Inverter protocol simulator for SIC Studio / Qt app testing.

Implements:
- TCP command channel on port 3030 (dictionary/read/write/stream/commit)
- UDP data stream on port 3040
- Register dictionary compatible with firmware/inverter/config/parameter.c

Usage example:
    python3 scripts/inverter_simulator.py --host 0.0.0.0 --tcp-port 3030 --udp-port 3040
"""

from __future__ import annotations

import argparse
import dataclasses
import logging
import math
import socket
import struct
import time
from typing import Dict, List, Optional, Tuple

START_BYTES = b"\xAA\x55"
MAX_PAYLOAD = 250
DICT_RECORD_LEN = 25

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


@dataclasses.dataclass(frozen=True)
class ParamDef:
    address: int
    type_code: int
    access: int
    group_id: int
    name: str
    unit: str
    default: int | float


@dataclasses.dataclass
class StreamState:
    stream_id: int
    loop_divider: int
    addresses: List[int]
    next_emit_tick: int
    seq: int = 0


@dataclasses.dataclass
class StreamStats:
    samples_sent: int = 0
    packets_sent: int = 0
    bytes_sent: int = 0
    last_report_s: float = 0.0


def clamp_float32(value: float) -> float:
    return struct.unpack("<f", struct.pack("<f", float(value)))[0]


def pad_ascii(text: str, width: int) -> bytes:
    data = text.encode("ascii", errors="ignore")[:width]
    return data.ljust(width, b" ")


def decode_typed(type_code: int, payload: bytes) -> int | float:
    _, fmt = TYPE_INFO[type_code]
    return struct.unpack(fmt, payload)[0]


def encode_typed(type_code: int, value: int | float) -> bytes:
    _, fmt = TYPE_INFO[type_code]

    if type_code == TYPE_FLOAT32:
        return struct.pack(fmt, clamp_float32(float(value)))

    if type_code == TYPE_UINT8:
        return struct.pack(fmt, int(value) & 0xFF)
    if type_code == TYPE_INT8:
        v = int(value)
        if v < -128 or v > 127:
            raise ValueError("int8 out of range")
        return struct.pack(fmt, v)
    if type_code == TYPE_UINT16:
        return struct.pack(fmt, int(value) & 0xFFFF)
    if type_code == TYPE_INT16:
        v = int(value)
        if v < -32768 or v > 32767:
            raise ValueError("int16 out of range")
        return struct.pack(fmt, v)
    if type_code == TYPE_UINT32:
        return struct.pack(fmt, int(value) & 0xFFFFFFFF)
    if type_code == TYPE_INT32:
        v = int(value)
        if v < -2147483648 or v > 2147483647:
            raise ValueError("int32 out of range")
        return struct.pack(fmt, v)
    if type_code == TYPE_UINT64:
        return struct.pack(fmt, int(value) & 0xFFFFFFFFFFFFFFFF)

    raise ValueError(f"unsupported type code {type_code}")


PARAMETERS: List[ParamDef] = [
    ParamDef(0x1000, TYPE_UINT16, ACCESS_WRITE_AFTER_RESTART, 0x01, "ADC_STEPS", "", 4096),
    ParamDef(0x1001, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x01, "ADC_SUPPLY", "mV", 5000.0),
    ParamDef(0x1100, TYPE_UINT16, ACCESS_WRITE_AFTER_RESTART, 0x02, "CUR_OFFS_U", "", 1361),
    ParamDef(0x1101, TYPE_UINT16, ACCESS_WRITE_AFTER_RESTART, 0x02, "CUR_OFFS_V", "", 1366),
    ParamDef(0x1102, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x02, "CUR_SENSE_FAC", "A/", 0.01),
    ParamDef(0x1200, TYPE_UINT32, ACCESS_WRITE_AFTER_RESTART, 0x03, "PWM_FREQ", "Hz", 20000),
    ParamDef(0x1300, TYPE_UINT8, ACCESS_WRITE_AFTER_RESTART, 0x04, "MOTOR_POLES", "", 7),
    ParamDef(0x1301, TYPE_FLOAT32, ACCESS_WRITE_LIVE, 0x04, "SPEED_REF", "Hz", 3.0),
    ParamDef(0x1302, TYPE_FLOAT32, ACCESS_WRITE_LIVE, 0x04, "VOLT_REF", "V", 5.0),
    ParamDef(0x1303, TYPE_UINT64, ACCESS_WRITE_AFTER_RESTART, 0x04, "CALIB_TICKS", "", 500000000),
    ParamDef(0x1304, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x04, "RES_OFFSET", "deg", 0.0),
    ParamDef(0x1305, TYPE_FLOAT32, ACCESS_WRITE_LIVE, 0x04, "ID_REF", "A", 0.0),
    ParamDef(0x1306, TYPE_FLOAT32, ACCESS_WRITE_LIVE, 0x04, "IQ_REF", "A", 4.0),
    ParamDef(0x1400, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x05, "ID_PI_KP", "", 0.05),
    ParamDef(0x1401, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x05, "ID_PI_KI", "", 0.05),
    ParamDef(0x1402, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x05, "ID_PI_MAX", "V", 0.5),
    ParamDef(0x1403, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x05, "ID_PI_MIN", "V", -0.5),
    ParamDef(0x1500, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x06, "IQ_PI_KP", "", 0.05),
    ParamDef(0x1501, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x06, "IQ_PI_KI", "", 0.05),
    ParamDef(0x1502, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x06, "IQ_PI_MAX", "V", 0.5),
    ParamDef(0x1503, TYPE_FLOAT32, ACCESS_WRITE_AFTER_RESTART, 0x06, "IQ_PI_MIN", "V", -0.5),
    ParamDef(0x1600, TYPE_UINT16, ACCESS_WRITE_AFTER_RESTART, 0x08, "RES_SIN_MAX", "", 4095),
    ParamDef(0x1601, TYPE_UINT16, ACCESS_WRITE_AFTER_RESTART, 0x08, "RES_SIN_MIN", "", 0),
    ParamDef(0x1602, TYPE_UINT16, ACCESS_WRITE_AFTER_RESTART, 0x08, "RES_COS_MAX", "", 4095),
    ParamDef(0x1603, TYPE_UINT16, ACCESS_WRITE_AFTER_RESTART, 0x08, "RES_COS_MIN", "", 0),
    ParamDef(0x1604, TYPE_UINT16, ACCESS_WRITE_AFTER_RESTART, 0x08, "RES_OFF_SIN", "", 2050),
    ParamDef(0x1605, TYPE_UINT16, ACCESS_WRITE_AFTER_RESTART, 0x08, "RES_OFF_COS", "", 2050),
    ParamDef(0x1606, TYPE_UINT8, ACCESS_WRITE_AFTER_RESTART, 0x08, "RES_POLES", "", 4),
    ParamDef(0x2000, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x22, "ELEC_ANGLE", "deg", 0.0),
    ParamDef(0x2001, TYPE_UINT8, ACCESS_READ_ONLY, 0x22, "IS_CALIB", "", 1),
    ParamDef(0x2100, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x23, "I_ALPHA", "A", 0.0),
    ParamDef(0x2101, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x23, "I_BETA", "A", 0.0),
    ParamDef(0x2102, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x23, "V_ALPHA", "V", 0.0),
    ParamDef(0x2103, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x23, "V_BETA", "V", 0.0),
    ParamDef(0x2104, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x23, "I_D", "A", 0.0),
    ParamDef(0x2105, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x23, "I_Q", "A", 0.0),
    ParamDef(0x2106, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x23, "V_D", "V", 0.0),
    ParamDef(0x2107, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x23, "V_Q", "V", 0.0),
    ParamDef(0x2200, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x24, "DUTY_U", "%", 50.0),
    ParamDef(0x2201, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x24, "DUTY_V", "%", 50.0),
    ParamDef(0x2202, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x24, "DUTY_W", "%", 50.0),
    ParamDef(0x2300, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x25, "ID_INTGL", "", 0.0),
    ParamDef(0x2301, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x25, "IQ_INTGL", "", 0.0),
    ParamDef(0x2400, TYPE_FLOAT32, ACCESS_READ_ONLY, 0x26, "PREV_ANGLE", "deg", 0.0),
    ParamDef(0x2401, TYPE_INT8, ACCESS_READ_ONLY, 0x26, "SECTOR", "", 0),
]


class InverterSimulator:
    def __init__(
        self,
        host: str,
        tcp_port: int,
        udp_port: int,
        loop_hz: float,
        tick_hz: float,
        logger: logging.Logger,
    ):
        self.host = host
        self.tcp_port = tcp_port
        self.udp_port = udp_port
        self.loop_hz = max(1.0, float(loop_hz))
        self.tick_hz = max(1.0, float(tick_hz))
        self.log = logger

        self.param_by_addr: Dict[int, ParamDef] = {p.address: p for p in PARAMETERS}
        self.live_values: Dict[int, int | float] = {p.address: p.default for p in PARAMETERS}
        self.shadow_values: Dict[int, int | float] = {
            p.address: p.default for p in PARAMETERS if p.access == ACCESS_WRITE_AFTER_RESTART
        }

        self.streams: Dict[int, StreamState] = {}
        self.stats = StreamStats(last_report_s=time.monotonic())
        self.cpu_start_s = time.monotonic()

        self.udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def _current_system_tick(self) -> int:
        return int((time.monotonic() - self.cpu_start_s) * self.tick_hz)

    def serve_forever(self) -> None:
        listen_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        listen_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        listen_sock.bind((self.host, self.tcp_port))
        listen_sock.listen(1)

        self.log.info("Simulator listening TCP %s:%d", self.host, self.tcp_port)
        self.log.info("Streaming UDP to client-ip:%d", self.udp_port)

        while True:
            conn, addr = listen_sock.accept()
            self.log.info("Control client connected: %s:%d", addr[0], addr[1])
            try:
                self._run_session(conn, addr[0])
            except (ConnectionError, OSError) as exc:
                self.log.info("Control client disconnected: %s", exc)
            finally:
                self.streams.clear()
                try:
                    conn.close()
                except OSError:
                    pass

    def _run_session(self, conn: socket.socket, peer_ip: str) -> None:
        conn.settimeout(0.001)
        rx_buf = bytearray()

        while True:
            self._update_dynamic_values()

            try:
                data = conn.recv(4096)
                if not data:
                    raise ConnectionError("peer closed")
                rx_buf.extend(data)
            except socket.timeout:
                pass

            for cmd, payload in self._extract_frames(rx_buf):
                self._handle_frame(conn, peer_ip, cmd, payload)

            self._emit_streams(peer_ip)
            self._report_stream_stats()

    def _extract_frames(self, rx_buf: bytearray) -> List[Tuple[int, bytes]]:
        out: List[Tuple[int, bytes]] = []

        while len(rx_buf) >= 5:
            if rx_buf[0] != START_BYTES[0] or rx_buf[1] != START_BYTES[1]:
                del rx_buf[0]
                continue

            cmd = rx_buf[2]
            payload_len = rx_buf[3] | (rx_buf[4] << 8)
            if payload_len > MAX_PAYLOAD:
                del rx_buf[0]
                continue

            frame_len = 5 + payload_len
            if len(rx_buf) < frame_len:
                break

            payload = bytes(rx_buf[5:frame_len])
            del rx_buf[:frame_len]
            out.append((cmd, payload))

        return out

    def _send_frame(self, conn: socket.socket, cmd: int, payload: bytes = b"") -> None:
        if len(payload) > MAX_PAYLOAD:
            raise ValueError("payload too large")
        frame = START_BYTES + bytes([cmd]) + struct.pack("<H", len(payload)) + payload
        conn.sendall(frame)

    def _send_error(self, conn: socket.socket, address: int, code: int) -> None:
        payload = struct.pack("<HB", address & 0xFFFF, code & 0xFF)
        self._send_frame(conn, CMD_ERROR, payload)

    def _handle_frame(self, conn: socket.socket, peer_ip: str, cmd: int, payload: bytes) -> None:
        if cmd == CMD_DICTIONARY:
            self._handle_dictionary(conn, payload)
            return
        if cmd == CMD_READ:
            self._handle_read(conn, payload)
            return
        if cmd == CMD_WRITE:
            self._handle_write(conn, payload)
            return
        if cmd == CMD_STREAM_START:
            self._handle_stream_start(conn, payload)
            return
        if cmd == CMD_STREAM_STOP:
            self._handle_stream_stop(conn, payload)
            return
        if cmd == CMD_CONFIG_COMMIT:
            self._handle_commit(conn, payload)
            return

        self.log.warning("Unknown command 0x%02X from %s", cmd, peer_ip)
        self._send_error(conn, 0x0000, ERR_OTHER)

    def _handle_dictionary(self, conn: socket.socket, payload: bytes) -> None:
        if payload:
            self._send_error(conn, 0x0000, ERR_OTHER)
            return

        records: List[bytes] = []
        for p in PARAMETERS:
            ctrl = (p.type_code & 0x0F) | ((p.access & 0x03) << 4)
            record = struct.pack("<HBB", p.address, ctrl, p.group_id)
            record += pad_ascii(p.name, 16)
            record += pad_ascii(p.unit, 5)
            records.append(record)

        chunk = bytearray()
        for rec in records:
            if len(chunk) + len(rec) > MAX_PAYLOAD:
                self._send_frame(conn, CMD_DICTIONARY_ACK, bytes(chunk))
                chunk.clear()
            chunk.extend(rec)
        if chunk:
            self._send_frame(conn, CMD_DICTIONARY_ACK, bytes(chunk))

    def _handle_read(self, conn: socket.socket, payload: bytes) -> None:
        if not payload or (len(payload) % 2) != 0:
            self._send_error(conn, 0x0000, ERR_OTHER)
            return

        response = bytearray()
        for i in range(0, len(payload), 2):
            address = payload[i] | (payload[i + 1] << 8)
            spec = self.param_by_addr.get(address)
            if spec is None:
                self._send_error(conn, address, ERR_INVALID_ADDR)
                return
            response.extend(encode_typed(spec.type_code, self.live_values[address]))

        self._send_frame(conn, CMD_READ_ACK, bytes(response))

    def _handle_write(self, conn: socket.socket, payload: bytes) -> None:
        if not payload:
            self._send_error(conn, 0x0000, ERR_OTHER)
            return

        offset = 0
        first_address: Optional[int] = None
        count = 0

        while offset < len(payload):
            if offset + 2 > len(payload):
                self._send_error(conn, 0x0000, ERR_OTHER)
                return

            address = payload[offset] | (payload[offset + 1] << 8)
            offset += 2

            spec = self.param_by_addr.get(address)
            if spec is None:
                self._send_error(conn, address, ERR_INVALID_ADDR)
                return

            if spec.access == ACCESS_READ_ONLY:
                self._send_error(conn, address, ERR_ACCESS_DENIED)
                return

            size, _ = TYPE_INFO[spec.type_code]
            if offset + size > len(payload):
                self._send_error(conn, 0x0000, ERR_OTHER)
                return

            raw = payload[offset: offset + size]
            offset += size

            value = decode_typed(spec.type_code, raw)

            if spec.access == ACCESS_WRITE_LIVE:
                self.live_values[address] = value
            else:
                self.shadow_values[address] = value

            if first_address is None:
                first_address = address
            count += 1

        ack = struct.pack("<HB", first_address or 0, count & 0xFF)
        self._send_frame(conn, CMD_WRITE_ACK, ack)

    def _handle_stream_start(self, conn: socket.socket, payload: bytes) -> None:
        if len(payload) < 4:
            self._send_error(conn, 0x0000, ERR_OTHER)
            return

        stream_id = payload[0]
        loop_divider = payload[1]
        addr_blob = payload[2:]

        if loop_divider == 0 or (len(addr_blob) % 2) != 0:
            self._send_error(conn, 0x0000, ERR_OTHER)
            return

        addresses: List[int] = []
        for i in range(0, len(addr_blob), 2):
            address = addr_blob[i] | (addr_blob[i + 1] << 8)
            if address not in self.param_by_addr:
                self._send_error(conn, address, ERR_INVALID_ADDR)
                return
            addresses.append(address)

        if not addresses:
            self._send_error(conn, 0x0000, ERR_OTHER)
            return

        now_tick = self._current_system_tick()
        self.streams[stream_id] = StreamState(
            stream_id=stream_id,
            loop_divider=loop_divider,
            addresses=addresses,
            next_emit_tick=now_tick,
        )

        ack = bytearray([stream_id, len(addresses)])
        for address in addresses:
            ack.extend(struct.pack("<H", address))
        self._send_frame(conn, CMD_STREAM_ACK, bytes(ack))

    def _handle_stream_stop(self, conn: socket.socket, payload: bytes) -> None:
        if len(payload) != 1:
            self._send_error(conn, 0x0000, ERR_OTHER)
            return

        stream_id = payload[0]
        if stream_id not in self.streams:
            self._send_error(conn, 0x0000, ERR_OTHER)
            return

        del self.streams[stream_id]
        self._send_frame(conn, CMD_STREAM_STOP_ACK, bytes([stream_id]))

    def _handle_commit(self, conn: socket.socket, payload: bytes) -> None:
        if payload:
            self._send_error(conn, 0x0000, ERR_OTHER)
            return

        for address, value in self.shadow_values.items():
            self.live_values[address] = value
        self._send_frame(conn, CMD_CONFIG_COMMIT_ACK, b"")

    def _dynamic_values_at(self, t: float) -> Dict[int, int | float]:
        values: Dict[int, int | float] = {}

        speed_ref_hz = float(self.live_values[0x1301])
        iq_ref = float(self.live_values[0x1306])
        id_ref = float(self.live_values[0x1305])

        angle_deg = (t * speed_ref_hz * 360.0) % 360.0
        angle_rad = math.radians(angle_deg)

        i_d = clamp_float32(id_ref + 0.2 * math.sin(2.0 * angle_rad))
        i_q = clamp_float32(iq_ref + 0.3 * math.cos(2.0 * angle_rad))

        v_d = clamp_float32(0.4 * i_d)
        v_q = clamp_float32(0.4 * i_q)

        i_alpha = clamp_float32(i_d * math.cos(angle_rad) - i_q * math.sin(angle_rad))
        i_beta = clamp_float32(i_d * math.sin(angle_rad) + i_q * math.cos(angle_rad))
        v_alpha = clamp_float32(v_d * math.cos(angle_rad) - v_q * math.sin(angle_rad))
        v_beta = clamp_float32(v_d * math.sin(angle_rad) + v_q * math.cos(angle_rad))

        duty_u = clamp_float32(50.0 + 20.0 * math.sin(angle_rad))
        duty_v = clamp_float32(50.0 + 20.0 * math.sin(angle_rad - (2.0 * math.pi / 3.0)))
        duty_w = clamp_float32(50.0 + 20.0 * math.sin(angle_rad + (2.0 * math.pi / 3.0)))

        integral_id = clamp_float32(0.5 * i_d + 0.5 * math.sin(0.4 * angle_rad))
        integral_iq = clamp_float32(0.5 * i_q + 0.5 * math.cos(0.4 * angle_rad))

        sector = int((angle_deg // 90.0) % 4)

        values[0x2000] = clamp_float32(angle_deg)
        values[0x2001] = 1
        values[0x2100] = i_alpha
        values[0x2101] = i_beta
        values[0x2102] = v_alpha
        values[0x2103] = v_beta
        values[0x2104] = i_d
        values[0x2105] = i_q
        values[0x2106] = v_d
        values[0x2107] = v_q
        values[0x2200] = duty_u
        values[0x2201] = duty_v
        values[0x2202] = duty_w
        values[0x2300] = integral_id
        values[0x2301] = integral_iq
        values[0x2400] = clamp_float32((angle_deg - speed_ref_hz * 0.25) % 360.0)
        values[0x2401] = sector
        return values

    def _update_dynamic_values(self) -> None:
        t = time.monotonic() - self.cpu_start_s
        self.live_values.update(self._dynamic_values_at(t))

    def _value_at_time(self, address: int, sample_time_s: float) -> int | float:
        spec = self.param_by_addr[address]
        if spec.access != ACCESS_READ_ONLY:
            return self.live_values[address]

        dynamic = self._dynamic_values_at(sample_time_s)
        return dynamic[address]

    def _emit_streams(self, peer_ip: str) -> None:
        now_tick = self._current_system_tick()
        period_tick = max(1, int(round(self.tick_hz / self.loop_hz)))

        for stream in list(self.streams.values()):
            stream_period_ticks = max(1, stream.loop_divider * period_tick)
            if now_tick < stream.next_emit_tick:
                continue

            sample_size = 0
            for address in stream.addresses:
                spec = self.param_by_addr[address]
                sample_size += TYPE_INFO[spec.type_code][0]

            sample_frame_size = 2 + 8 + sample_size
            max_samples_per_packet = max(1, (MAX_PAYLOAD - 4) // sample_frame_size)

            samples_to_emit = ((now_tick - stream.next_emit_tick) // stream_period_ticks) + 1
            while samples_to_emit > 0:
                emit_now = min(samples_to_emit, max_samples_per_packet)
                datagram = bytearray(START_BYTES)
                datagram.append(stream.stream_id)
                datagram.append(emit_now)

                for _ in range(emit_now):
                    tick = stream.next_emit_tick & 0xFFFFFFFFFFFFFFFF
                    sample_time_s = tick / self.tick_hz

                    datagram.extend(struct.pack("<H", stream.seq & 0xFFFF))
                    datagram.extend(struct.pack("<Q", tick))

                    for address in stream.addresses:
                        spec = self.param_by_addr[address]
                        value = self._value_at_time(address, sample_time_s)
                        datagram.extend(encode_typed(spec.type_code, value))

                    stream.seq = (stream.seq + 1) & 0xFFFF
                    stream.next_emit_tick += stream_period_ticks

                data = bytes(datagram)
                self.udp_sock.sendto(data, (peer_ip, self.udp_port))
                self.stats.packets_sent += 1
                self.stats.samples_sent += emit_now
                self.stats.bytes_sent += len(data)
                samples_to_emit -= emit_now

    def _report_stream_stats(self) -> None:
        now = time.monotonic()
        dt = now - self.stats.last_report_s
        if dt < 1.0:
            return

        samples_per_s = self.stats.samples_sent / dt
        packets_per_s = self.stats.packets_sent / dt
        mbit_per_s = (self.stats.bytes_sent * 8.0) / (dt * 1_000_000.0)

        if self.streams:
            self.log.info(
                "Stream throughput: %.0f samples/s, %.0f packets/s, %.2f Mbit/s (%d active streams)",
                samples_per_s,
                packets_per_s,
                mbit_per_s,
                len(self.streams),
            )

        self.stats.samples_sent = 0
        self.stats.packets_sent = 0
        self.stats.bytes_sent = 0
        self.stats.last_report_s = now


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Inverter protocol simulator for SIC Studio")
    parser.add_argument("--host", default="0.0.0.0", help="TCP listen host (default: 0.0.0.0)")
    parser.add_argument("--tcp-port", type=int, default=3030, help="TCP command port (default: 3030)")
    parser.add_argument("--udp-port", type=int, default=3040, help="UDP stream destination port (default: 3040)")
    parser.add_argument(
        "--loop-hz",
        type=float,
        default=100000.0,
        help="Simulated control-loop frequency used for stream loopDivider timing (default: 100000)",
    )
    parser.add_argument(
        "--tick-hz",
        type=float,
        default=100000000.0,
        help="Timestamp tick frequency for UDP samples (default: 100000000)",
    )
    parser.add_argument("--verbose", action="store_true", help="Enable debug logging")
    return parser


def main() -> int:
    args = build_arg_parser().parse_args()
    logging.basicConfig(
        level=logging.DEBUG if args.verbose else logging.INFO,
        format="%(asctime)s [%(levelname)s] %(message)s",
    )
    logger = logging.getLogger("inverter-sim")

    simulator = InverterSimulator(
        host=args.host,
        tcp_port=args.tcp_port,
        udp_port=args.udp_port,
        loop_hz=args.loop_hz,
        tick_hz=args.tick_hz,
        logger=logger,
    )

    try:
        simulator.serve_forever()
    except KeyboardInterrupt:
        logger.info("Simulator stopped")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())