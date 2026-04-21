#ifndef INVERTERCLIENT_H
#define INVERTERCLIENT_H

#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QVector>
#include <QElapsedTimer>

#include "dictionarymodel.h"
#include "plotsample.h"

class InverterClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool streamActive READ streamActive NOTIFY streamActiveChanged)

public:
    explicit InverterClient(DictionaryModel *model, QObject *parent = nullptr);

    bool connected() const;
    QString status() const;
    bool streamActive() const;

    Q_INVOKABLE void connectToDevice(const QString &host, int port = 3030);
    Q_INVOKABLE void disconnectFromDevice();

    Q_INVOKABLE void requestDictionary();
    Q_INVOKABLE void readAddresses(const QVariantList &addresses);
    Q_INVOKABLE void readAllDictionaryValues();
    Q_INVOKABLE void writeValue(int address, const QString &valueText);
    Q_INVOKABLE void startStream(int streamId, int loopDivider, const QVariantList &addresses);
    Q_INVOKABLE void stopStream(int streamId);
    Q_INVOKABLE void stopActiveStream();
    Q_INVOKABLE void commitConfig();

signals:
    void connectedChanged();
    void statusChanged();
    void streamActiveChanged();
    void dictionaryReloaded();
    void streamSample(int address, QString name, double tSec, double value);
    void streamSamplesReady(const QVector<PlotSample> &samples);
    void commandError(QString text);

private slots:
    void onTcpConnected();
    void onTcpDisconnected();
    void onTcpError(QAbstractSocket::SocketError socketError);
    void onTcpReadyRead();
    void onUdpReadyRead();
    void finalizeDictionary();
    void flushPendingStreamValues();

private:
    static constexpr quint8 Start0 = 0xAA;
    static constexpr quint8 Start1 = 0x55;
    static constexpr quint8 CmdDictionaryReq = 0x00;
    static constexpr quint8 CmdReadReq = 0x01;
    static constexpr quint8 CmdWriteReq = 0x02;
    static constexpr quint8 CmdStreamReq = 0x03;
    static constexpr quint8 CmdStreamStop = 0x04;
    static constexpr quint8 CmdUpdateConfig = 0x06;

    static constexpr quint8 CmdDictionaryData = 0x80;
    static constexpr quint8 CmdReadData = 0x81;
    static constexpr quint8 CmdWriteAck = 0x82;
    static constexpr quint8 CmdStreamAck = 0x83;
    static constexpr quint8 CmdStreamStopAck = 0x84;
    static constexpr quint8 CmdUpdateConfigAck = 0x86;
    static constexpr quint8 CmdError = 0x8F;

    static constexpr quint16 UdpPort = 3040;
    static constexpr int DictRecordLen = 24;

    DictionaryModel *m_model;
    QTcpSocket m_tcp;
    QUdpSocket m_udp;
    QTimer m_dictFinalizeTimer;
    QTimer m_streamUiFlushTimer;

    QByteArray m_tcpBuffer;
    QString m_status;

    QVector<DictionaryModel::Entry> m_dictBuilding;
    QVector<quint16> m_pendingReadAddresses;

    QHash<quint8, QVector<quint16>> m_streamAddressOrder;
    QHash<quint8, int> m_streamSampleSize;
    QHash<quint8, quint16> m_lastSequence;
    QHash<quint8, quint64> m_firstStreamTick;
    QHash<quint8, quint64> m_lastStreamTick;
    QHash<quint16, QVariant> m_pendingStreamUiValues;
    int m_activeStreamId = -1;
    bool m_streamActive = false;
    QElapsedTimer m_streamClock;
    bool m_streamClockStarted = false;
    double m_streamTicksPerSecond = 100000000.0;

    void setStatus(const QString &text);
    void setStreamActive(bool active);
    void resetStreamRuntimeState();

    void sendFrame(quint8 cmd, const QByteArray &payload);
    void processTcpBuffer();
    void handleFrame(quint8 cmd, const QByteArray &payload);

    bool decodeValue(const QByteArray &blob, int offset, quint8 type, QVariant *outValue, int *outLen) const;
    bool encodeValue(quint8 type, const QString &text, QByteArray *outBytes) const;

    void handleDictionaryChunk(const QByteArray &payload);
    void handleReadData(const QByteArray &payload);
    void handleWriteAck(const QByteArray &payload);
    void handleStreamAck(const QByteArray &payload);
    void handleErrorFrame(const QByteArray &payload);
    void handleUdpDatagram(const QByteArray &datagram);
};

#endif