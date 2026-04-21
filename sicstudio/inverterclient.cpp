#include "inverterclient.h"

#include <QDataStream>
#include <QtEndian>

namespace {
QString trimAscii(const QByteArray &bytes)
{
    return QString::fromLatin1(bytes).trimmed();
}
}

InverterClient::InverterClient(DictionaryModel *model, QObject *parent)
    : QObject(parent)
    , m_model(model)
{
    connect(&m_tcp, &QTcpSocket::connected, this, &InverterClient::onTcpConnected);
    connect(&m_tcp, &QTcpSocket::disconnected, this, &InverterClient::onTcpDisconnected);
    connect(&m_tcp, &QTcpSocket::errorOccurred, this, &InverterClient::onTcpError);
    connect(&m_tcp, &QTcpSocket::readyRead, this, &InverterClient::onTcpReadyRead);

    connect(&m_udp, &QUdpSocket::readyRead, this, &InverterClient::onUdpReadyRead);

    m_dictFinalizeTimer.setSingleShot(true);
    m_dictFinalizeTimer.setInterval(250);
    connect(&m_dictFinalizeTimer, &QTimer::timeout, this, &InverterClient::finalizeDictionary);

    // Stream samples can arrive far faster than the UI can redraw table rows.
    // Coalesce per-address value updates and flush at a bounded rate.
    m_streamUiFlushTimer.setSingleShot(true);
    m_streamUiFlushTimer.setInterval(50);
    connect(&m_streamUiFlushTimer, &QTimer::timeout, this, &InverterClient::flushPendingStreamValues);

    setStatus(QStringLiteral("Disconnected"));
}

bool InverterClient::connected() const
{
    return m_tcp.state() == QAbstractSocket::ConnectedState;
}

QString InverterClient::status() const
{
    return m_status;
}

bool InverterClient::streamActive() const
{
    return m_streamActive;
}

void InverterClient::connectToDevice(const QString &host, int port)
{
    if (host.trimmed().isEmpty()) {
        setStatus(QStringLiteral("Host is empty"));
        return;
    }

    if (connected()) {
        m_tcp.disconnectFromHost();
    }

    m_tcpBuffer.clear();
    m_dictBuilding.clear();
    m_pendingReadAddresses.clear();
    setStatus(QStringLiteral("Connecting to %1:%2").arg(host).arg(port));
    m_tcp.connectToHost(host, static_cast<quint16>(port));
}

void InverterClient::disconnectFromDevice()
{
    m_tcp.disconnectFromHost();
    m_udp.close();
    resetStreamRuntimeState();
    setStatus(QStringLiteral("Disconnected"));
}

void InverterClient::requestDictionary()
{
    if (!connected()) {
        setStatus(QStringLiteral("Not connected"));
        return;
    }

    m_dictBuilding.clear();
    m_dictFinalizeTimer.stop();
    sendFrame(CmdDictionaryReq, {});
    setStatus(QStringLiteral("Dictionary request sent"));
}

void InverterClient::readAddresses(const QVariantList &addresses)
{
    if (!connected()) {
        setStatus(QStringLiteral("Not connected"));
        return;
    }

    QByteArray payload;
    payload.reserve(addresses.size() * 2);

    QVector<quint16> requested;
    requested.reserve(addresses.size());

    for (const QVariant &v : addresses) {
        bool ok = false;
        const int iv = v.toInt(&ok);
        if (!ok || iv < 0 || iv > 0xFFFF) {
            continue;
        }

        const quint16 address = static_cast<quint16>(iv);
        payload.append(static_cast<char>(address & 0xFF));
        payload.append(static_cast<char>((address >> 8) & 0xFF));
        requested.push_back(address);
    }

    if (requested.isEmpty()) {
        setStatus(QStringLiteral("No valid addresses to read"));
        return;
    }

    m_pendingReadAddresses = requested;
    sendFrame(CmdReadReq, payload);
    setStatus(QStringLiteral("Read request: %1 item(s)").arg(requested.size()));
}

void InverterClient::readAllDictionaryValues()
{
    QVariantList addresses;
    const int rows = m_model != nullptr ? m_model->rowCount() : 0;
    for (int i = 0; i < rows; ++i) {
        addresses.push_back(m_model->addressAt(i));
    }
    readAddresses(addresses);
}

void InverterClient::writeValue(int address, const QString &valueText)
{
    if (!connected()) {
        setStatus(QStringLiteral("Not connected"));
        return;
    }

    if (address < 0 || address > 0xFFFF) {
        setStatus(QStringLiteral("Invalid address"));
        return;
    }

    quint8 type = 0;
    quint8 access = 0;
    if (m_model == nullptr || !m_model->metadataForAddress(static_cast<quint16>(address), &type, &access, nullptr, nullptr)) {
        setStatus(QStringLiteral("Address 0x%1 not in dictionary").arg(address, 4, 16, QLatin1Char('0')));
        return;
    }

    if (access == DictionaryModel::AccessReadOnly) {
        setStatus(QStringLiteral("Address is read-only"));
        return;
    }

    QByteArray rawValue;
    if (!encodeValue(type, valueText, &rawValue)) {
        setStatus(QStringLiteral("Cannot parse value for type %1").arg(DictionaryModel::typeName(type)));
        return;
    }

    QByteArray payload;
    payload.reserve(2 + rawValue.size());
    const quint16 addr = static_cast<quint16>(address);
    payload.append(static_cast<char>(addr & 0xFF));
    payload.append(static_cast<char>((addr >> 8) & 0xFF));
    payload.append(rawValue);

    sendFrame(CmdWriteReq, payload);
    setStatus(QStringLiteral("Write request: 0x%1").arg(addr, 4, 16, QLatin1Char('0')));
}

void InverterClient::startStream(int streamId, int loopDivider, const QVariantList &addresses)
{
    if (!connected()) {
        setStatus(QStringLiteral("Not connected"));
        return;
    }

    if (streamId < 0 || streamId > 255 || loopDivider <= 0 || loopDivider > 255) {
        setStatus(QStringLiteral("Invalid stream id or loop divider"));
        return;
    }

    if (m_activeStreamId >= 0 && m_activeStreamId != streamId) {
        const quint8 previousStreamId = static_cast<quint8>(m_activeStreamId);
        QByteArray stopPayload;
        stopPayload.append(static_cast<char>(previousStreamId));
        sendFrame(CmdStreamStop, stopPayload);
    }

    resetStreamRuntimeState();
    setStreamActive(false);

    QByteArray payload;
    payload.reserve(2 + addresses.size() * 2);

    payload.append(static_cast<char>(streamId & 0xFF));
    payload.append(static_cast<char>(loopDivider & 0xFF));

    int count = 0;
    for (const QVariant &v : addresses) {
        bool ok = false;
        const int addrInt = v.toInt(&ok);
        if (!ok || addrInt < 0 || addrInt > 0xFFFF) {
            continue;
        }

        const quint16 addr = static_cast<quint16>(addrInt);
        payload.append(static_cast<char>(addr & 0xFF));
        payload.append(static_cast<char>((addr >> 8) & 0xFF));
        ++count;
    }

    if (count == 0) {
        setStatus(QStringLiteral("Select at least one stream address"));
        return;
    }

    // Drop previous decode state immediately so in-flight old packets are ignored during stream reconfiguration.
    m_streamAddressOrder.remove(static_cast<quint8>(streamId));
    m_streamSampleSize.remove(static_cast<quint8>(streamId));
    m_lastSequence.remove(static_cast<quint8>(streamId));
    m_firstStreamTick.remove(static_cast<quint8>(streamId));
    m_lastStreamTick.remove(static_cast<quint8>(streamId));
    m_pendingStreamUiValues.clear();
    m_pendingReadAddresses.clear();
    m_activeStreamId = streamId;

    sendFrame(CmdStreamReq, payload);
    setStatus(QStringLiteral("Stream request: id=%1 divider=%2 count=%3")
                  .arg(streamId)
                  .arg(loopDivider)
                  .arg(count));
}

void InverterClient::stopStream(int streamId)
{
    if (!connected()) {
        setStatus(QStringLiteral("Not connected"));
        return;
    }

    if (streamId < 0 || streamId > 255) {
        setStatus(QStringLiteral("Invalid stream id"));
        return;
    }

    QByteArray payload;
    payload.append(static_cast<char>(streamId & 0xFF));

    m_streamAddressOrder.remove(static_cast<quint8>(streamId));
    m_streamSampleSize.remove(static_cast<quint8>(streamId));
    m_lastSequence.remove(static_cast<quint8>(streamId));
    m_firstStreamTick.remove(static_cast<quint8>(streamId));
    m_lastStreamTick.remove(static_cast<quint8>(streamId));
    m_pendingStreamUiValues.clear();
    m_pendingReadAddresses.clear();
    if (m_activeStreamId == streamId) {
        m_activeStreamId = -1;
        setStreamActive(false);
    }

    sendFrame(CmdStreamStop, payload);
    setStatus(QStringLiteral("Stream stop request: id=%1").arg(streamId));
}

void InverterClient::stopActiveStream()
{
    if (m_activeStreamId < 0 || m_activeStreamId > 255) {
        setStatus(QStringLiteral("No active stream to stop"));
        return;
    }

    stopStream(m_activeStreamId);
}

void InverterClient::commitConfig()
{
    if (!connected()) {
        setStatus(QStringLiteral("Not connected"));
        return;
    }

    sendFrame(CmdUpdateConfig, {});
    setStatus(QStringLiteral("Update-config request sent"));
}

void InverterClient::onTcpConnected()
{
    if (!m_udp.bind(QHostAddress::AnyIPv4, UdpPort,
                    QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        setStatus(QStringLiteral("TCP connected, UDP bind failed: %1").arg(m_udp.errorString()));
    } else {
        setStatus(QStringLiteral("Connected (TCP + UDP %1)").arg(UdpPort));
    }

    m_streamClock.invalidate();
    m_streamClockStarted = false;

    emit connectedChanged();
}

void InverterClient::onTcpDisconnected()
{
    m_udp.close();
    resetStreamRuntimeState();
    setStatus(QStringLiteral("Disconnected"));
    emit connectedChanged();
}

void InverterClient::resetStreamRuntimeState()
{
    m_streamAddressOrder.clear();
    m_streamSampleSize.clear();
    m_lastSequence.clear();
    m_firstStreamTick.clear();
    m_lastStreamTick.clear();
    m_pendingStreamUiValues.clear();
    m_pendingReadAddresses.clear();
    m_streamUiFlushTimer.stop();
    m_streamClock.invalidate();
    m_streamClockStarted = false;
    m_activeStreamId = -1;
    setStreamActive(false);
}

void InverterClient::setStreamActive(bool active)
{
    if (m_streamActive == active) {
        return;
    }

    m_streamActive = active;
    emit streamActiveChanged();
}

void InverterClient::flushPendingStreamValues()
{
    if (m_model == nullptr || m_pendingStreamUiValues.isEmpty()) {
        return;
    }

    for (auto it = m_pendingStreamUiValues.cbegin(); it != m_pendingStreamUiValues.cend(); ++it) {
        m_model->updateValue(it.key(), it.value());
    }
    m_pendingStreamUiValues.clear();
}

void InverterClient::onTcpError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    setStatus(QStringLiteral("Socket error: %1").arg(m_tcp.errorString()));
}

void InverterClient::onTcpReadyRead()
{
    m_tcpBuffer.append(m_tcp.readAll());
    processTcpBuffer();
}

void InverterClient::onUdpReadyRead()
{
    while (m_udp.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(m_udp.pendingDatagramSize()));
        m_udp.readDatagram(datagram.data(), datagram.size());
        handleUdpDatagram(datagram);
    }
}

void InverterClient::finalizeDictionary()
{
    if (m_dictBuilding.isEmpty()) {
        return;
    }

    if (m_model != nullptr) {
        m_model->setEntries(m_dictBuilding);
    }

    const int count = m_dictBuilding.size();
    m_dictBuilding.clear();
    setStatus(QStringLiteral("Dictionary loaded: %1 entries").arg(count));
    emit dictionaryReloaded();
}

void InverterClient::setStatus(const QString &text)
{
    if (m_status == text) {
        return;
    }

    m_status = text;
    emit statusChanged();
}

void InverterClient::sendFrame(quint8 cmd, const QByteArray &payload)
{
    if (!connected()) {
        return;
    }

    QByteArray frame;
    frame.reserve(5 + payload.size());

    frame.append(static_cast<char>(Start0));
    frame.append(static_cast<char>(Start1));
    frame.append(static_cast<char>(cmd));

    const quint16 length = static_cast<quint16>(payload.size());
    frame.append(static_cast<char>(length & 0xFF));
    frame.append(static_cast<char>((length >> 8) & 0xFF));
    frame.append(payload);

    m_tcp.write(frame);
}

void InverterClient::processTcpBuffer()
{
    while (m_tcpBuffer.size() >= 5) {
        int startIndex = -1;
        for (int i = 0; i < m_tcpBuffer.size() - 1; ++i) {
            if (static_cast<quint8>(m_tcpBuffer.at(i)) == Start0 && static_cast<quint8>(m_tcpBuffer.at(i + 1)) == Start1) {
                startIndex = i;
                break;
            }
        }

        if (startIndex < 0) {
            m_tcpBuffer.clear();
            return;
        }

        if (startIndex > 0) {
            m_tcpBuffer.remove(0, startIndex);
            if (m_tcpBuffer.size() < 5) {
                return;
            }
        }

        const quint16 payloadLen = static_cast<quint8>(m_tcpBuffer.at(3))
            | (static_cast<quint16>(static_cast<quint8>(m_tcpBuffer.at(4))) << 8);
        const int frameLen = 5 + payloadLen;
        if (m_tcpBuffer.size() < frameLen) {
            return;
        }

        const quint8 cmd = static_cast<quint8>(m_tcpBuffer.at(2));
        const QByteArray payload = m_tcpBuffer.mid(5, payloadLen);
        m_tcpBuffer.remove(0, frameLen);

        handleFrame(cmd, payload);
    }
}

void InverterClient::handleFrame(quint8 cmd, const QByteArray &payload)
{
    switch (cmd) {
    case CmdDictionaryData:
        handleDictionaryChunk(payload);
        return;
    case CmdReadData:
        handleReadData(payload);
        return;
    case CmdWriteAck:
        handleWriteAck(payload);
        return;
    case CmdStreamAck:
        handleStreamAck(payload);
        return;
    case CmdStreamStopAck:
        if (payload.size() == 1) {
            const int streamId = static_cast<quint8>(payload.at(0));
            m_streamAddressOrder.remove(static_cast<quint8>(streamId));
            m_streamSampleSize.remove(static_cast<quint8>(streamId));
            m_lastSequence.remove(static_cast<quint8>(streamId));
            m_firstStreamTick.remove(static_cast<quint8>(streamId));
            m_lastStreamTick.remove(static_cast<quint8>(streamId));
            if (m_activeStreamId == streamId) {
                m_activeStreamId = -1;
                setStreamActive(false);
            }
            setStatus(QStringLiteral("Stream stopped: id=%1").arg(streamId));
        }
        return;
    case CmdUpdateConfigAck:
        setStatus(QStringLiteral("Configuration committed"));
        return;
    case CmdError:
        handleErrorFrame(payload);
        return;
    default:
        setStatus(QStringLiteral("Unhandled response cmd=0x%1")
                      .arg(cmd, 2, 16, QLatin1Char('0')));
        return;
    }
}

bool InverterClient::decodeValue(const QByteArray &blob, int offset, quint8 type, QVariant *outValue, int *outLen) const
{
    const int size = DictionaryModel::byteSizeForType(type);
    if (size <= 0 || offset < 0 || offset + size > blob.size()) {
        return false;
    }

    const uchar *data = reinterpret_cast<const uchar *>(blob.constData() + offset);

    switch (type) {
    case DictionaryModel::TypeUInt8:
        *outValue = QVariant::fromValue(static_cast<quint32>(data[0]));
        break;
    case DictionaryModel::TypeInt8:
        *outValue = QVariant::fromValue(static_cast<qint32>(static_cast<qint8>(data[0])));
        break;
    case DictionaryModel::TypeUInt16:
        *outValue = QVariant::fromValue(static_cast<quint32>(qFromLittleEndian<quint16>(data)));
        break;
    case DictionaryModel::TypeInt16:
        *outValue = QVariant::fromValue(static_cast<qint32>(qFromLittleEndian<qint16>(data)));
        break;
    case DictionaryModel::TypeUInt32:
        *outValue = QVariant::fromValue(qFromLittleEndian<quint32>(data));
        break;
    case DictionaryModel::TypeInt32:
        *outValue = QVariant::fromValue(qFromLittleEndian<qint32>(data));
        break;
    case DictionaryModel::TypeFloat32: {
        const quint32 raw = qFromLittleEndian<quint32>(data);
        float f = 0.0f;
        static_assert(sizeof(float) == sizeof(quint32), "float size mismatch");
        memcpy(&f, &raw, sizeof(float));
        *outValue = QVariant::fromValue(static_cast<double>(f));
        break;
    }
    case DictionaryModel::TypeUInt64:
        *outValue = QVariant::fromValue(qFromLittleEndian<quint64>(data));
        break;
    default:
        return false;
    }

    if (outLen != nullptr) {
        *outLen = size;
    }
    return true;
}

bool InverterClient::encodeValue(quint8 type, const QString &text, QByteArray *outBytes) const
{
    if (outBytes == nullptr) {
        return false;
    }

    outBytes->clear();
    outBytes->reserve(DictionaryModel::byteSizeForType(type));

    bool ok = false;
    switch (type) {
    case DictionaryModel::TypeUInt8: {
        const uint v = text.toUInt(&ok, 0);
        if (!ok || v > 0xFF) {
            return false;
        }
        outBytes->append(static_cast<char>(v & 0xFF));
        return true;
    }
    case DictionaryModel::TypeInt8: {
        const int v = text.toInt(&ok, 0);
        if (!ok || v < -128 || v > 127) {
            return false;
        }
        outBytes->append(static_cast<char>(static_cast<qint8>(v)));
        return true;
    }
    case DictionaryModel::TypeUInt16: {
        const uint v = text.toUInt(&ok, 0);
        if (!ok || v > 0xFFFF) {
            return false;
        }
        outBytes->append(static_cast<char>(v & 0xFF));
        outBytes->append(static_cast<char>((v >> 8) & 0xFF));
        return true;
    }
    case DictionaryModel::TypeInt16: {
        const int v = text.toInt(&ok, 0);
        if (!ok || v < -32768 || v > 32767) {
            return false;
        }
        const quint16 raw = static_cast<quint16>(static_cast<qint16>(v));
        outBytes->append(static_cast<char>(raw & 0xFF));
        outBytes->append(static_cast<char>((raw >> 8) & 0xFF));
        return true;
    }
    case DictionaryModel::TypeUInt32: {
        const quint32 v = text.toUInt(&ok, 0);
        if (!ok) {
            return false;
        }
        outBytes->append(static_cast<char>(v & 0xFF));
        outBytes->append(static_cast<char>((v >> 8) & 0xFF));
        outBytes->append(static_cast<char>((v >> 16) & 0xFF));
        outBytes->append(static_cast<char>((v >> 24) & 0xFF));
        return true;
    }
    case DictionaryModel::TypeInt32: {
        const qint32 v = text.toInt(&ok, 0);
        if (!ok) {
            return false;
        }
        const quint32 raw = static_cast<quint32>(v);
        outBytes->append(static_cast<char>(raw & 0xFF));
        outBytes->append(static_cast<char>((raw >> 8) & 0xFF));
        outBytes->append(static_cast<char>((raw >> 16) & 0xFF));
        outBytes->append(static_cast<char>((raw >> 24) & 0xFF));
        return true;
    }
    case DictionaryModel::TypeFloat32: {
        const float v = text.toFloat(&ok);
        if (!ok) {
            return false;
        }
        quint32 raw = 0;
        static_assert(sizeof(float) == sizeof(quint32), "float size mismatch");
        memcpy(&raw, &v, sizeof(float));
        outBytes->append(static_cast<char>(raw & 0xFF));
        outBytes->append(static_cast<char>((raw >> 8) & 0xFF));
        outBytes->append(static_cast<char>((raw >> 16) & 0xFF));
        outBytes->append(static_cast<char>((raw >> 24) & 0xFF));
        return true;
    }
    case DictionaryModel::TypeUInt64: {
        const quint64 v = text.toULongLong(&ok, 0);
        if (!ok) {
            return false;
        }
        for (int i = 0; i < 8; ++i) {
            outBytes->append(static_cast<char>((v >> (8 * i)) & 0xFF));
        }
        return true;
    }
    default:
        return false;
    }
}

void InverterClient::handleDictionaryChunk(const QByteArray &payload)
{
    if (payload.size() % DictRecordLen != 0) {
        setStatus(QStringLiteral("Invalid dictionary payload length: %1").arg(payload.size()));
        return;
    }

    for (int i = 0; i < payload.size(); i += DictRecordLen) {
        const uchar *rec = reinterpret_cast<const uchar *>(payload.constData() + i);

        DictionaryModel::Entry entry;
        entry.address = qFromLittleEndian<quint16>(rec);
        const quint8 ctrl = rec[2];
        entry.type = ctrl & 0x0F;
        entry.access = (ctrl >> 4) & 0x03;
        entry.name = trimAscii(payload.mid(i + 3, 16));
        entry.unit = trimAscii(payload.mid(i + 19, 5));
        entry.value = QVariant();
        entry.plotEnabled = false;
        m_dictBuilding.push_back(entry);
    }

    m_dictFinalizeTimer.start();
}

void InverterClient::handleReadData(const QByteArray &payload)
{
    if (m_model == nullptr) {
        return;
    }

    int offset = 0;
    for (int i = 0; i < m_pendingReadAddresses.size(); ++i) {
        const quint16 addr = m_pendingReadAddresses[i];

        quint8 type = 0;
        if (!m_model->metadataForAddress(addr, &type, nullptr, nullptr, nullptr)) {
            setStatus(QStringLiteral("Read decode failed: unknown address 0x%1")
                          .arg(addr, 4, 16, QLatin1Char('0')));
            return;
        }

        QVariant decoded;
        int readLen = 0;
        if (!decodeValue(payload, offset, type, &decoded, &readLen)) {
            setStatus(QStringLiteral("Read decode failed at 0x%1")
                          .arg(addr, 4, 16, QLatin1Char('0')));
            return;
        }

        offset += readLen;
        m_model->updateValue(addr, decoded);
    }

    if (offset != payload.size()) {
        setStatus(QStringLiteral("Read response length mismatch"));
    } else {
        setStatus(QStringLiteral("Read response parsed: %1 item(s)").arg(m_pendingReadAddresses.size()));
    }

    m_pendingReadAddresses.clear();
}

void InverterClient::handleWriteAck(const QByteArray &payload)
{
    if (payload.size() != 3) {
        setStatus(QStringLiteral("Invalid write-ack payload"));
        return;
    }

    const quint16 firstAddr = static_cast<quint8>(payload.at(0))
        | (static_cast<quint16>(static_cast<quint8>(payload.at(1))) << 8);
    const int count = static_cast<quint8>(payload.at(2));
    setStatus(QStringLiteral("Write acknowledged: first=0x%1 count=%2")
                  .arg(firstAddr, 4, 16, QLatin1Char('0'))
                  .arg(count));
}

void InverterClient::handleStreamAck(const QByteArray &payload)
{
    if (payload.size() < 2) {
        setStatus(QStringLiteral("Invalid stream-ack payload"));
        return;
    }

    const quint8 streamId = static_cast<quint8>(payload.at(0));
    const int count = static_cast<quint8>(payload.at(1));

    if (payload.size() != 2 + (count * 2)) {
        setStatus(QStringLiteral("Stream-ack length mismatch"));
        return;
    }

    QVector<quint16> addresses;
    addresses.reserve(count);
    int sampleSizeBytes = 0;
    for (int i = 0; i < count; ++i) {
        const int p = 2 + (i * 2);
        const quint16 addr = static_cast<quint8>(payload.at(p))
            | (static_cast<quint16>(static_cast<quint8>(payload.at(p + 1))) << 8);

        int valueSize = 0;
        if (m_model == nullptr) {
            setStatus(QStringLiteral("Stream-ack without dictionary model"));
            return;
        }

        valueSize = m_model->byteSizeForAddress(addr);
        if (valueSize <= 0) {
            setStatus(QStringLiteral("Stream-ack contains unknown address 0x%1")
                          .arg(addr, 4, 16, QLatin1Char('0')));
            return;
        }

        sampleSizeBytes += valueSize;
        addresses.push_back(addr);
    }

    m_streamAddressOrder.insert(streamId, addresses);
    m_streamSampleSize.insert(streamId, sampleSizeBytes);
    m_lastSequence.remove(streamId);
    m_firstStreamTick.remove(streamId);
    m_lastStreamTick.remove(streamId);
    if (m_activeStreamId == static_cast<int>(streamId)) {
        setStreamActive(true);
    }
    setStatus(QStringLiteral("Stream active: id=%1 count=%2").arg(streamId).arg(count));
}

void InverterClient::handleErrorFrame(const QByteArray &payload)
{
    if (payload.size() != 3) {
        setStatus(QStringLiteral("Malformed error frame"));
        return;
    }

    const quint16 address = static_cast<quint8>(payload.at(0))
        | (static_cast<quint16>(static_cast<quint8>(payload.at(1))) << 8);
    const quint8 code = static_cast<quint8>(payload.at(2));

    QString codeText;
    switch (code) {
    case 0x01:
        codeText = QStringLiteral("Invalid Address");
        break;
    case 0x02:
        codeText = QStringLiteral("Access Denied");
        break;
    case 0x03:
    default:
        codeText = QStringLiteral("Other/Malformed");
        break;
    }

    const QString msg = QStringLiteral("Device error: addr=0x%1 code=0x%2 (%3)")
                            .arg(address, 4, 16, QLatin1Char('0'))
                            .arg(code, 2, 16, QLatin1Char('0'))
                            .arg(codeText);
    setStatus(msg);
    emit commandError(msg);
}

void InverterClient::handleUdpDatagram(const QByteArray &datagram)
{
    if (datagram.size() < 4) {
        return;
    }

    const uchar *d = reinterpret_cast<const uchar *>(datagram.constData());
    if (d[0] != Start0 || d[1] != Start1) {
        return;
    }

    const quint8 streamId = d[2];
    const quint8 sampleCount = d[3];

    const QVector<quint16> addresses = m_streamAddressOrder.value(streamId);
    if (addresses.isEmpty()) {
        return;
    }

    if (sampleCount == 0U) {
        return;
    }

    const int sampleSize = m_streamSampleSize.value(streamId, 0);
    if (sampleSize <= 0) {
        return;
    }

    const int expectedBytes = 4 + static_cast<int>(sampleCount) * (2 + 8 + sampleSize);
    if (datagram.size() != expectedBytes) {
        // Can happen transiently when stream layout changes; ignore without status spam.
        return;
    }

    if (!m_streamClockStarted) {
        m_streamClock.start();
        m_streamClockStarted = true;
    }

    QVector<PlotSample> batch;
    batch.reserve(static_cast<int>(sampleCount) * addresses.size());

    int offset = 4;
    for (int sampleIndex = 0; sampleIndex < static_cast<int>(sampleCount); ++sampleIndex) {
        const quint16 seq = qFromLittleEndian<quint16>(d + offset);
        offset += 2;

        const quint64 tick = qFromLittleEndian<quint64>(d + offset);
        offset += 8;

        const quint16 last = m_lastSequence.value(streamId, static_cast<quint16>(seq - 1));
        if (static_cast<quint16>(last + 1) != seq) {
            setStatus(QStringLiteral("UDP seq jump on stream %1: %2 -> %3")
                          .arg(streamId)
                          .arg(last)
                          .arg(seq));
        }
        m_lastSequence.insert(streamId, seq);

        quint64 tick0 = m_firstStreamTick.value(streamId, tick);
        bool rebaseTickOrigin = false;
        if (!m_firstStreamTick.contains(streamId)) {
            m_firstStreamTick.insert(streamId, tick);
            tick0 = tick;
            rebaseTickOrigin = true;
        }

        if (m_lastStreamTick.contains(streamId) && tick < m_lastStreamTick.value(streamId)) {
            rebaseTickOrigin = true;
        }

        if (tick < tick0) {
            rebaseTickOrigin = true;
        }

        if (rebaseTickOrigin) {
            m_firstStreamTick.insert(streamId, tick);
            tick0 = tick;
        }

        m_lastStreamTick.insert(streamId, tick);
        const double tSec = static_cast<double>(tick - tick0) / m_streamTicksPerSecond;

        for (int i = 0; i < addresses.size(); ++i) {
            const quint16 addr = addresses[i];

            quint8 type = 0;
            QString name;
            if (m_model == nullptr || !m_model->metadataForAddress(addr, &type, nullptr, &name, nullptr)) {
                return;
            }

            QVariant decoded;
            int len = 0;
            if (!decodeValue(datagram, offset, type, &decoded, &len)) {
                return;
            }
            offset += len;

            m_pendingStreamUiValues.insert(addr, decoded);

            PlotSample sample;
            sample.address = static_cast<int>(addr);
            sample.name = name;
            sample.tSec = tSec;
            sample.value = decoded.toDouble();
            batch.push_back(sample);
        }
    }

    if (!batch.isEmpty()) {
        if (!m_streamUiFlushTimer.isActive()) {
            m_streamUiFlushTimer.start();
        }
        emit streamSamplesReady(batch);
    }
}