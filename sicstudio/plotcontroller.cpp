#include "plotcontroller.h"

#include <QGuiApplication>
#include <QScreen>
#include <cmath>
#include <algorithm>
#include <QtMath>
#include <QMetaObject>

PlotWorker::PlotWorker(QObject *parent)
    : QObject(parent)
{
}

void PlotWorker::initialize()
{
    if (m_frameTimer != nullptr) {
        return;
    }

    m_frameTimer = new QTimer(this);
    m_frameTimer->setTimerType(Qt::PreciseTimer);
    m_frameTimer->setInterval(qMax(4, 1000 / m_targetFps));
    connect(m_frameTimer, &QTimer::timeout, this, &PlotWorker::publishFrame);
    m_frameTimer->start();
}

void PlotWorker::enqueueSample(int address, const QString &name, double tSec, double value)
{
    if (!m_tracked.contains(address)) {
        return;
    }

    if (!std::isfinite(tSec) || !std::isfinite(value) || tSec < 0.0) {
        return;
    }

    // Qt Graphs may internally downcast to float; keep values in a safe range.
    constexpr double MaxPlottableAbs = 1.0e9;
    value = std::clamp(value, -MaxPlottableAbs, MaxPlottableAbs);

    SeriesState &state = m_series[address];
    state.name = name;
    state.latestX = tSec;
    m_lastX = qMax(m_lastX, tSec);

    const double duration = bucketDuration();
    const qint64 bucketIndex = static_cast<qint64>(qFloor(tSec / duration));
    if (!state.bucketActive) {
        state.bucketActive = true;
        state.bucketIndex = bucketIndex;
        state.bucketX = tSec;
        state.bucketLastX = tSec;
        state.bucketMin = value;
        state.bucketMax = value;
        state.bucketSum = value;
        state.bucketCount = 1;
    } else if (state.bucketIndex != bucketIndex) {
        flushBucket(state);
        state.bucketActive = true;
        state.bucketIndex = bucketIndex;
        state.bucketX = tSec;
        state.bucketLastX = tSec;
        state.bucketMin = value;
        state.bucketMax = value;
        state.bucketSum = value;
        state.bucketCount = 1;
    } else {
        state.bucketLastX = tSec;
        state.bucketMin = qMin(state.bucketMin, value);
        state.bucketMax = qMax(state.bucketMax, value);
        state.bucketSum += value;
        ++state.bucketCount;
    }

    if (!state.initialized) {
        state.initialized = true;
        state.minY = value;
        state.maxY = value;
    } else {
        state.minY = qMin(state.minY, value);
        state.maxY = qMax(state.maxY, value);
    }

    state.dirty = true;
    m_dirtyAddresses.insert(address);
}

void PlotWorker::enqueueSamples(const QVector<PlotSample> &samples)
{
    for (const PlotSample &sample : samples) {
        enqueueSample(sample.address, sample.name, sample.tSec, sample.value);
    }
}

void PlotWorker::setTrackedSeries(const QVariantList &series)
{
    QSet<int> tracked;
    for (const QVariant &entryVariant : series) {
        const QVariantMap entry = entryVariant.toMap();
        const int address = entry.value(QStringLiteral("address")).toInt();
        if (address < 0) {
            continue;
        }

        tracked.insert(address);
        SeriesState &state = m_series[address];
        const QString name = entry.value(QStringLiteral("name")).toString();
        if (!name.isEmpty()) {
            state.name = name;
        }
    }

    const auto keys = m_series.keys();
    for (int address : keys) {
        if (!tracked.contains(address)) {
            m_series.remove(address);
            m_dirtyAddresses.remove(address);
        }
    }

    m_tracked = tracked;
}

void PlotWorker::setWindowSeconds(double seconds)
{
    if (seconds <= 0.1) {
        return;
    }
    m_windowSeconds = seconds;
}

void PlotWorker::setRenderWindowSeconds(double seconds)
{
    if (seconds <= 0.05) {
        return;
    }
    m_renderWindowSeconds = seconds;
}

void PlotWorker::setMaxPointsPerSeries(int maxPoints)
{
    m_maxPointsPerSeries = qBound(100, maxPoints, 10000);
}

void PlotWorker::setTargetFps(int fps)
{
    m_targetFps = qBound(10, fps, 240);
    if (m_frameTimer != nullptr) {
        m_frameTimer->setInterval(qMax(4, 1000 / m_targetFps));
    }
}

void PlotWorker::setRenderingEnabled(bool enabled)
{
    m_renderingEnabled = enabled;
}

void PlotWorker::clear()
{
    m_series.clear();
    m_dirtyAddresses.clear();
    m_lastX = 0.0;
    emit cleared();
}

void PlotWorker::publishFrame()
{
    if (m_dirtyAddresses.isEmpty()) {
        return;
    }

    if (!m_renderingEnabled) {
        const double keepFromX = qMax(0.0, m_lastX - m_windowSeconds);
        for (int address : std::as_const(m_dirtyAddresses)) {
            if (m_series.contains(address)) {
                trimOldPoints(m_series[address], keepFromX);
            }
        }
        m_dirtyAddresses.clear();
        return;
    }

    const double xMax = qMax(m_windowSeconds, m_lastX);
    const double xMin = qMax(0.0, xMax - m_windowSeconds);

    QVariantList frameSeries;
    frameSeries.reserve(m_dirtyAddresses.size());

    double yMin = 0.0;
    double yMax = 0.0;
    bool yInit = false;

    for (int address : std::as_const(m_dirtyAddresses)) {
        if (!m_series.contains(address)) {
            continue;
        }

        SeriesState &state = m_series[address];
        flushBucket(state);
        trimOldPoints(state, xMin);

        QVariantList pointList;
        pointList.reserve(state.points.size());

        for (const QPointF &pt : std::as_const(state.points)) {
            pointList.push_back(pt);
            if (!yInit) {
                yInit = true;
                yMin = pt.y();
                yMax = pt.y();
            } else {
                yMin = qMin(yMin, pt.y());
                yMax = qMax(yMax, pt.y());
            }
        }

        QVariantMap packed;
        packed.insert(QStringLiteral("address"), address);
        packed.insert(QStringLiteral("name"), state.name);
        packed.insert(QStringLiteral("points"), pointList);
        frameSeries.push_back(packed);
        state.dirty = false;
    }

    m_dirtyAddresses.clear();

    if (!yInit) {
        yMin = -1.0;
        yMax = 1.0;
    } else if (qFuzzyCompare(yMin + 1.0, yMax + 1.0)) {
        const double margin = qAbs(yMin) < 1e-6 ? 0.1 : qAbs(yMin) * 0.1;
        yMin -= margin;
        yMax += margin;
    } else {
        const double margin = (yMax - yMin) * 0.08;
        yMin -= margin;
        yMax += margin;
    }

    emit frameReady(frameSeries, xMin, xMax, yMin, yMax);
}

double PlotWorker::bucketDuration() const
{
    return qMax(0.0001, m_renderWindowSeconds / static_cast<double>(m_maxPointsPerSeries));
}

void PlotWorker::pushBucketPoint(SeriesState &state, double x, double y)
{
    state.points.push_back(QPointF(x, y));
}

void PlotWorker::flushBucket(SeriesState &state)
{
    if (!state.bucketActive) {
        return;
    }

    const double average = state.bucketCount > 0
        ? state.bucketSum / static_cast<double>(state.bucketCount)
        : (state.bucketMin + state.bucketMax) * 0.5;
    pushBucketPoint(state, state.bucketLastX, average);

    state.bucketActive = false;
    state.bucketIndex = -1;
    state.bucketSum = 0.0;
    state.bucketCount = 0;
}

void PlotWorker::trimOldPoints(SeriesState &state, double keepFromX)
{
    int removeCount = 0;
    while (removeCount < state.points.size() && state.points[removeCount].x() < keepFromX) {
        ++removeCount;
    }

    if (removeCount > 0) {
        state.points.remove(0, removeCount);
    }

    if (state.points.size() > m_maxPointsPerSeries * 2) {
        state.points.remove(0, state.points.size() - (m_maxPointsPerSeries * 2));
    }
}

PlotController::PlotController(QObject *parent)
    : QObject(parent)
{
    m_worker = new PlotWorker();
    m_worker->moveToThread(&m_workerThread);

    connect(&m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_worker, &PlotWorker::frameReady, this, &PlotController::onFrameReady, Qt::QueuedConnection);
    connect(m_worker, &PlotWorker::cleared, this, &PlotController::cleared, Qt::QueuedConnection);

    m_workerThread.start();

    QMetaObject::invokeMethod(m_worker, "initialize", Qt::QueuedConnection);

    QMetaObject::invokeMethod(m_worker, "setWindowSeconds", Qt::QueuedConnection, Q_ARG(double, m_windowSeconds));
    QMetaObject::invokeMethod(m_worker, "setRenderWindowSeconds", Qt::QueuedConnection, Q_ARG(double, m_renderWindowSeconds));
    QMetaObject::invokeMethod(m_worker, "setMaxPointsPerSeries", Qt::QueuedConnection, Q_ARG(int, m_maxPointsPerSeries));
    QMetaObject::invokeMethod(m_worker, "setTargetFps", Qt::QueuedConnection, Q_ARG(int, m_targetFps));
}

PlotController::~PlotController()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

double PlotController::xMin() const
{
    return m_xMin;
}

double PlotController::xMax() const
{
    return m_xMax;
}

double PlotController::yMin() const
{
    return m_yMin;
}

double PlotController::yMax() const
{
    return m_yMax;
}

double PlotController::windowSeconds() const
{
    return m_windowSeconds;
}

double PlotController::renderWindowSeconds() const
{
    return m_renderWindowSeconds;
}

int PlotController::maxPointsPerSeries() const
{
    return m_maxPointsPerSeries;
}

int PlotController::targetFps() const
{
    return m_targetFps;
}

bool PlotController::renderingEnabled() const
{
    return m_renderingEnabled;
}

void PlotController::setSeriesEnabled(int address, const QString &name, bool enabled)
{
    if (address < 0) {
        return;
    }

    if (enabled) {
        m_enabledSeries.insert(address, name);
    } else {
        m_enabledSeries.remove(address);
        emit seriesFrame(address, name, {});
    }

    pushTrackedSeriesToWorker();
}

void PlotController::clear()
{
    QMetaObject::invokeMethod(m_worker, "clear", Qt::QueuedConnection);
}

void PlotController::syncTargetFpsToPrimaryScreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen == nullptr) {
        setTargetFps(60);
        return;
    }

    const qreal rate = screen->refreshRate();
    if (!std::isfinite(rate) || rate < 1.0) {
        setTargetFps(60);
        return;
    }

    const int refresh = qRound(rate);
    setTargetFps(refresh > 1 ? refresh : 60);
}

void PlotController::ingestSample(int address, const QString &name, double tSec, double value)
{
    QMetaObject::invokeMethod(m_worker,
                              "enqueueSample",
                              Qt::QueuedConnection,
                              Q_ARG(int, address),
                              Q_ARG(QString, name),
                              Q_ARG(double, tSec),
                              Q_ARG(double, value));
}

void PlotController::ingestSamples(const QVector<PlotSample> &samples)
{
    QMetaObject::invokeMethod(m_worker,
                              "enqueueSamples",
                              Qt::QueuedConnection,
                              Q_ARG(QVector<PlotSample>, samples));
}

void PlotController::setWindowSeconds(double seconds)
{
    const double clamped = qBound(1.0, seconds, 120.0);
    if (qFuzzyCompare(m_windowSeconds + 1.0, clamped + 1.0)) {
        return;
    }

    m_windowSeconds = clamped;
    emit windowSecondsChanged();

    QMetaObject::invokeMethod(m_worker, "setWindowSeconds", Qt::QueuedConnection, Q_ARG(double, m_windowSeconds));
}

void PlotController::setRenderWindowSeconds(double seconds)
{
    const double clamped = qBound(0.1, seconds, 120.0);
    if (qFuzzyCompare(m_renderWindowSeconds + 1.0, clamped + 1.0)) {
        return;
    }

    m_renderWindowSeconds = clamped;
    emit renderWindowSecondsChanged();

    QMetaObject::invokeMethod(m_worker, "setRenderWindowSeconds", Qt::QueuedConnection, Q_ARG(double, m_renderWindowSeconds));
}

void PlotController::setMaxPointsPerSeries(int maxPoints)
{
    const int clamped = qBound(100, maxPoints, 10000);
    if (m_maxPointsPerSeries == clamped) {
        return;
    }

    m_maxPointsPerSeries = clamped;
    emit maxPointsPerSeriesChanged();

    QMetaObject::invokeMethod(m_worker, "setMaxPointsPerSeries", Qt::QueuedConnection, Q_ARG(int, m_maxPointsPerSeries));
}

void PlotController::setTargetFps(int fps)
{
    const int clamped = qBound(10, fps, 240);
    if (m_targetFps == clamped) {
        return;
    }

    m_targetFps = clamped;
    emit targetFpsChanged();

    QMetaObject::invokeMethod(m_worker, "setTargetFps", Qt::QueuedConnection, Q_ARG(int, m_targetFps));
}

void PlotController::setRenderingEnabled(bool enabled)
{
    if (m_renderingEnabled == enabled) {
        return;
    }

    m_renderingEnabled = enabled;
    emit renderingEnabledChanged();

    QMetaObject::invokeMethod(m_worker,
                              "setRenderingEnabled",
                              Qt::QueuedConnection,
                              Q_ARG(bool, m_renderingEnabled));
}

void PlotController::onFrameReady(const QVariantList &seriesFrames, double xMin, double xMax, double yMin, double yMax)
{
    m_xMin = xMin;
    m_xMax = xMax;
    m_yMin = yMin;
    m_yMax = yMax;
    emit axesChanged();

    for (const QVariant &seriesVariant : seriesFrames) {
        const QVariantMap packed = seriesVariant.toMap();
        const int address = packed.value(QStringLiteral("address")).toInt();
        const QString name = packed.value(QStringLiteral("name")).toString();
        const QVariantList points = packed.value(QStringLiteral("points")).toList();
        emit seriesFrame(address, name, points);
    }
}

void PlotController::pushTrackedSeriesToWorker()
{
    QVariantList tracked;
    tracked.reserve(m_enabledSeries.size());

    for (auto it = m_enabledSeries.cbegin(); it != m_enabledSeries.cend(); ++it) {
        QVariantMap entry;
        entry.insert(QStringLiteral("address"), it.key());
        entry.insert(QStringLiteral("name"), it.value());
        tracked.push_back(entry);
    }

    QMetaObject::invokeMethod(m_worker,
                              "setTrackedSeries",
                              Qt::QueuedConnection,
                              Q_ARG(QVariantList, tracked));
}
