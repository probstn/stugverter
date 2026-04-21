#include "plotcontroller.h"

#include <QGuiApplication>
#include <QScreen>
#include <cmath>
#include <algorithm>
#include <QtMath>
#include <QMetaObject>
#include <limits>

PlotWorker::PlotWorker(QObject *parent)
    : QObject(parent)
{
}

void PlotWorker::schedulePublishFrame()
{
    if (m_framePublishQueued) {
        ++m_coalescedFrameRequests;
        return;
    }

    m_framePublishQueued = true;
    QMetaObject::invokeMethod(this, &PlotWorker::publishFrame, Qt::QueuedConnection);
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
    state.rawPoints.push_back(QPointF(tSec, value));
    m_lastX = qMax(m_lastX, tSec);
    ++m_ingestedSamplesWindow;

    state.dirty = true;
    m_dirtyAddresses.insert(address);
}

void PlotWorker::enqueueSamples(const QVector<PlotSample> &samples)
{
    for (const PlotSample &sample : samples) {
        enqueueSample(sample.address, sample.name, sample.tSec, sample.value);
    }
}

void PlotWorker::requestFrame()
{
    const auto keys = m_series.keys();
    for (int address : keys) {
        m_dirtyAddresses.insert(address);
    }
    m_forceFrameRequested = true;
    schedulePublishFrame();
}

void PlotWorker::requestViewFrame(double xMin, double xMax)
{
    if (!std::isfinite(xMin) || !std::isfinite(xMax) || xMax <= xMin) {
        return;
    }

    m_manualXMin = qMax(0.0, xMin);
    m_manualXMax = qMax(m_manualXMin + 0.0001, xMax);
    m_hasManualView = true;
    requestFrame();
}

void PlotWorker::clearViewFrame()
{
    if (!m_hasManualView) {
        return;
    }

    m_hasManualView = false;
    requestFrame();
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
    if (seconds <= 0.01) {
        return;
    }
    m_windowSeconds = seconds;
}

void PlotWorker::setRenderWindowSeconds(double seconds)
{
    if (seconds <= 0.01) {
        return;
    }
    m_visibleWindowSeconds = seconds;
}

void PlotWorker::setRetentionSeconds(double seconds)
{
    if (seconds <= 0.05) {
        return;
    }
    m_retentionSeconds = seconds;
}

void PlotWorker::setMaxPointsPerSeries(int maxPoints)
{
    m_maxPointsPerSeries = qBound(100, maxPoints, 10000);
}

void PlotWorker::setDisplayPointBudget(int points)
{
    m_displayPointBudget = qBound(200, points, 40000);
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
    if (m_renderingEnabled) {
        requestFrame();
    }
}

void PlotWorker::clear()
{
    m_series.clear();
    m_dirtyAddresses.clear();
    m_lastX = 0.0;
    m_hasManualView = false;
    m_manualXMin = 0.0;
    m_manualXMax = 10.0;
    m_ingestedSamplesWindow = 0;
    m_outputPointsWindow = 0;
    m_frameCountWindow = 0;
    m_inputSamplesPerSecond = 0.0;
    m_outputPointsPerSecond = 0.0;
    m_renderFramesPerSecond = 0.0;
    m_lastFrameBuildMs = 0.0;
    m_avgFrameBuildMs = 0.0;
    m_displayedPointCount = 0;
    m_decimationRatio = 1.0;
    m_coalescedFrameRequests = 0;
    m_telemetryClock.invalidate();
    emit cleared();
}

void PlotWorker::publishFrame()
{
    m_framePublishQueued = false;
    QElapsedTimer frameBuildTimer;
    frameBuildTimer.start();

    if (m_dirtyAddresses.isEmpty() && !m_forceFrameRequested) {
        return;
    }

    const double keepFromX = qMax(0.0, m_lastX - m_retentionSeconds);
    bool hasBufferedData = false;
    double bufferMinX = 0.0;
    double bufferMaxX = 0.0;
    bool minSpacingInit = false;
    double minSampleSpacing = 0.001;

    for (auto it = m_series.begin(); it != m_series.end(); ++it) {
        SeriesState &state = it.value();
        trimOldPoints(state, keepFromX);
        if (state.rawPoints.isEmpty()) {
            continue;
        }

        const double seriesFirstX = state.rawPoints.first().x();
        const double seriesLastX = state.rawPoints.last().x();
        if (!hasBufferedData) {
            hasBufferedData = true;
            bufferMinX = seriesFirstX;
            bufferMaxX = seriesLastX;
        } else {
            bufferMinX = qMin(bufferMinX, seriesFirstX);
            bufferMaxX = qMax(bufferMaxX, seriesLastX);
        }

        const int firstProbeIndex = qMax(1, state.rawPoints.size() - SpacingProbeIntervals);
        for (int i = firstProbeIndex; i < state.rawPoints.size(); ++i) {
            const double dt = state.rawPoints[i].x() - state.rawPoints[i - 1].x();
            if (!std::isfinite(dt) || dt <= 0.0) {
                continue;
            }

            if (!minSpacingInit || dt < minSampleSpacing) {
                minSampleSpacing = dt;
                minSpacingInit = true;
            }
        }
    }

    if (!hasBufferedData) {
        bufferMinX = 0.0;
        bufferMaxX = qMax(m_visibleWindowSeconds, m_windowSeconds);
    }

    if (!m_renderingEnabled) {
        m_dirtyAddresses.clear();
        m_forceFrameRequested = false;
        return;
    }

    const double visibleSpan = qMax(0.01, m_visibleWindowSeconds);
    double xMin = 0.0;
    double xMax = visibleSpan;

    if (m_hasManualView) {
        xMin = m_manualXMin;
        xMax = qMax(m_manualXMin + 0.0001, m_manualXMax);

        if (hasBufferedData) {
            if (xMin < bufferMinX) {
                const double shift = bufferMinX - xMin;
                xMin += shift;
                xMax += shift;
            }
            if (xMax > bufferMaxX) {
                const double shift = xMax - bufferMaxX;
                xMin -= shift;
                xMax -= shift;
            }
            xMin = qMax(bufferMinX, xMin);
            xMax = qMax(xMin + 0.0001, qMin(bufferMaxX, xMax));
        }
    } else if (hasBufferedData) {
        xMax = bufferMaxX;
        xMin = qMax(bufferMinX, xMax - visibleSpan);
    }

    QVariantList frameSeries;
    frameSeries.reserve(m_dirtyAddresses.size());

    struct PendingSeries {
        int address = -1;
        SeriesState *state = nullptr;
        int rawVisibleCount = 0;
        int targetCount = 0;
    };

    QVector<PendingSeries> pendingSeries;
    pendingSeries.reserve(m_dirtyAddresses.size());

    auto countVisiblePoints = [](const QVector<QPointF> &rawPoints, double xMinValue, double xMaxValue) {
        if (rawPoints.isEmpty() || xMaxValue <= xMinValue) {
            return 0;
        }

        const auto begin = rawPoints.cbegin();
        const auto end = rawPoints.cend();

        const auto firstIt = std::lower_bound(begin, end, xMinValue, [](const QPointF &point, double value) {
            return point.x() < value;
        });
        const auto pastLastIt = std::upper_bound(begin, end, xMaxValue, [](double value, const QPointF &point) {
            return value < point.x();
        });

        const int firstVisible = static_cast<int>(firstIt - begin);
        const int lastVisible = static_cast<int>(pastLastIt - begin) - 1;
        if (lastVisible < firstVisible) {
            return 0;
        }

        const int expandedFirst = qMax(0, firstVisible - 1);
        const int expandedLast = qMin(rawPoints.size() - 1, lastVisible + 1);
        return expandedLast - expandedFirst + 1;
    };

    double yMin = 0.0;
    double yMax = 0.0;
    bool yInit = false;
    int rawVisiblePointTotal = 0;

    for (int address : std::as_const(m_dirtyAddresses)) {
        if (!m_series.contains(address)) {
            continue;
        }

        SeriesState &state = m_series[address];

        if (state.rawPoints.isEmpty()) {
            continue;
        }

        const int seriesRawVisibleCount = countVisiblePoints(state.rawPoints, xMin, xMax);
        rawVisiblePointTotal += seriesRawVisibleCount;

        PendingSeries pending;
        pending.address = address;
        pending.state = &state;
        pending.rawVisibleCount = seriesRawVisibleCount;
        pendingSeries.push_back(pending);
    }

    const int visibleSeriesCount = std::count_if(pendingSeries.cbegin(), pendingSeries.cend(), [](const PendingSeries &pending) {
        return pending.rawVisibleCount > 0;
    });

    int remainingBudget = qBound(200, m_displayPointBudget, 40000);
    int remainingVisibleSeries = visibleSeriesCount;
    int displayedPointTotal = 0;

    for (PendingSeries &pending : pendingSeries) {
        if (pending.rawVisibleCount <= 0) {
            continue;
        }

        const int fairShare = qMax(1, remainingBudget / qMax(1, remainingVisibleSeries));
        pending.targetCount = qMin(qMin(m_maxPointsPerSeries, pending.rawVisibleCount), fairShare);
        remainingBudget -= pending.targetCount;
        --remainingVisibleSeries;
    }

    for (PendingSeries &pending : pendingSeries) {
        SeriesState &state = *pending.state;

        double seriesYMin = 0.0;
        double seriesYMax = 0.0;
        int seriesRawVisibleCount = 0;
        QVariantList pointList = buildDisplayPoints(state.rawPoints,
                                                    xMin,
                                                    xMax,
                                                    &seriesYMin,
                                                    &seriesYMax,
                                                    &seriesRawVisibleCount,
                                                    pending.targetCount);

        if (!yInit && !pointList.isEmpty()) {
            yInit = true;
            yMin = seriesYMin;
            yMax = seriesYMax;
        } else if (!pointList.isEmpty()) {
            yMin = qMin(yMin, seriesYMin);
            yMax = qMax(yMax, seriesYMax);
        }

        QVariantMap packed;
        packed.insert(QStringLiteral("address"), pending.address);
        packed.insert(QStringLiteral("name"), state.name);
        packed.insert(QStringLiteral("points"), pointList);
        frameSeries.push_back(packed);
        displayedPointTotal += pointList.size();
        state.dirty = false;
    }

    m_dirtyAddresses.clear();
    m_forceFrameRequested = false;

    m_lastFrameBuildMs = static_cast<double>(frameBuildTimer.nsecsElapsed()) / 1.0e6;
    if (m_avgFrameBuildMs <= 0.0) {
        m_avgFrameBuildMs = m_lastFrameBuildMs;
    } else {
        m_avgFrameBuildMs = (m_avgFrameBuildMs * 0.9) + (m_lastFrameBuildMs * 0.1);
    }
    m_displayedPointCount = displayedPointTotal;
    m_decimationRatio = displayedPointTotal > 0
        ? static_cast<double>(rawVisiblePointTotal) / static_cast<double>(displayedPointTotal)
        : 1.0;
    m_outputPointsWindow += displayedPointTotal;
    ++m_frameCountWindow;

    if (!m_telemetryClock.isValid()) {
        m_telemetryClock.start();
    }
    const qint64 telemetryElapsedMs = m_telemetryClock.elapsed();
    if (telemetryElapsedMs >= 500) {
        const double secs = static_cast<double>(telemetryElapsedMs) / 1000.0;
        m_inputSamplesPerSecond = static_cast<double>(m_ingestedSamplesWindow) / secs;
        m_outputPointsPerSecond = static_cast<double>(m_outputPointsWindow) / secs;
        m_renderFramesPerSecond = static_cast<double>(m_frameCountWindow) / secs;
        m_ingestedSamplesWindow = 0;
        m_outputPointsWindow = 0;
        m_frameCountWindow = 0;
        m_telemetryClock.restart();
    }

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

    emit frameReady(frameSeries,
                    xMin,
                    xMax,
                    yMin,
                    yMax,
                    bufferMinX,
                    bufferMaxX,
                    minSampleSpacing,
                    m_inputSamplesPerSecond,
                    m_outputPointsPerSecond,
                    m_renderFramesPerSecond,
                    m_lastFrameBuildMs,
                    m_avgFrameBuildMs,
                    m_displayedPointCount,
                    m_decimationRatio,
                    m_coalescedFrameRequests);
}

QVariantList PlotWorker::buildDisplayPoints(const QVector<QPointF> &rawPoints,
                                            double xMin,
                                            double xMax,
                                            double *yMin,
                                            double *yMax,
                                            int *rawVisibleCount,
                                            int targetCount) const
{
    QVariantList points;
    if (rawPoints.isEmpty() || xMax <= xMin) {
        if (rawVisibleCount != nullptr) {
            *rawVisibleCount = 0;
        }
        return points;
    }

    const auto begin = rawPoints.cbegin();
    const auto end = rawPoints.cend();

    const auto firstIt = std::lower_bound(begin, end, xMin, [](const QPointF &point, double value) {
        return point.x() < value;
    });
    const auto pastLastIt = std::upper_bound(begin, end, xMax, [](double value, const QPointF &point) {
        return value < point.x();
    });

    const int firstVisible = static_cast<int>(firstIt - begin);
    const int lastVisible = static_cast<int>(pastLastIt - begin) - 1;

    if (lastVisible < firstVisible) {
        return points;
    }

    const int expandedFirst = qMax(0, firstVisible - 1);
    const int expandedLast = qMin(rawPoints.size() - 1, lastVisible + 1);
    const int visibleCount = expandedLast - expandedFirst + 1;
    if (rawVisibleCount != nullptr) {
        *rawVisibleCount = visibleCount;
    }

    targetCount = qMax(1, qMin(m_maxPointsPerSeries, targetCount));

    if (visibleCount <= targetCount) {
        points.reserve(visibleCount);
        bool firstPoint = true;
        for (int i = expandedFirst; i <= expandedLast; ++i) {
            const QPointF &pt = rawPoints[i];
            points.push_back(pt);
            if (firstPoint) {
                *yMin = pt.y();
                *yMax = pt.y();
                firstPoint = false;
            } else {
                *yMin = qMin(*yMin, pt.y());
                *yMax = qMax(*yMax, pt.y());
            }
        }
        return points;
    }

    const int buckets = qMax(1, targetCount / 2);
    const double span = qMax(0.000001, xMax - xMin);
    const double bucketWidth = span / static_cast<double>(buckets);

    struct BucketAgg {
        bool has = false;
        int minIndex = -1;
        int maxIndex = -1;
    };
    QVector<BucketAgg> bucketAggs(buckets);

    const int safeLast = qMin(expandedLast, rawPoints.size() - 1);
    for (int i = expandedFirst; i <= safeLast; ++i) {
        const QPointF &pt = rawPoints[i];
        int bucketIndex = static_cast<int>((pt.x() - xMin) / bucketWidth);
        if (bucketIndex < 0) {
            bucketIndex = 0;
        } else if (bucketIndex >= buckets) {
            bucketIndex = buckets - 1;
        }

        BucketAgg &agg = bucketAggs[bucketIndex];
        if (!agg.has) {
            agg.has = true;
            agg.minIndex = i;
            agg.maxIndex = i;
            continue;
        }

        if (rawPoints[i].y() < rawPoints[agg.minIndex].y()) {
            agg.minIndex = i;
        }
        if (rawPoints[i].y() > rawPoints[agg.maxIndex].y()) {
            agg.maxIndex = i;
        }
    }

    points.reserve(qMin(visibleCount, buckets * 2 + 2));
    bool firstPoint = true;

    for (const BucketAgg &agg : std::as_const(bucketAggs)) {
        if (!agg.has) {
            continue;
        }

        int firstIndex = agg.minIndex;
        int secondIndex = agg.maxIndex;
        if (firstIndex > secondIndex) {
            std::swap(firstIndex, secondIndex);
        }

        const QPointF firstPointInBucket = rawPoints[firstIndex];
        const QPointF secondPointInBucket = rawPoints[secondIndex];

        if (points.isEmpty() || points.back() != firstPointInBucket) {
            points.push_back(firstPointInBucket);
        }
        if (secondIndex != firstIndex) {
            points.push_back(secondPointInBucket);
        }

        const double bucketMin = qMin(firstPointInBucket.y(), secondPointInBucket.y());
        const double bucketMax = qMax(firstPointInBucket.y(), secondPointInBucket.y());
        if (firstPoint) {
            *yMin = bucketMin;
            *yMax = bucketMax;
            firstPoint = false;
        } else {
            *yMin = qMin(*yMin, bucketMin);
            *yMax = qMax(*yMax, bucketMax);
        }
    }

    if (!points.isEmpty()) {
        const QPointF firstPoint = rawPoints[expandedFirst];
        const QPointF lastPoint = rawPoints[expandedLast];
        if (points.first() != firstPoint) {
            points.prepend(firstPoint);
        }
        if (points.last() != lastPoint) {
            points.push_back(lastPoint);
        }
    }

    for (const QVariant &value : std::as_const(points)) {
        const QPointF pt = value.toPointF();
        *yMin = qMin(*yMin, pt.y());
        *yMax = qMax(*yMax, pt.y());
    }

    return points;
}

void PlotWorker::trimOldPoints(SeriesState &state, double keepFromX)
{
    int removeCount = 0;
    while (removeCount < state.rawPoints.size() && state.rawPoints[removeCount].x() < keepFromX) {
        ++removeCount;
    }

    if (removeCount > 0) {
        state.rawPoints.remove(0, removeCount);
    }

    const int storageCap = qMax(20000, m_maxPointsPerSeries * StorageOversampling * 4);
    if (state.rawPoints.size() > storageCap) {
        state.rawPoints.remove(0, state.rawPoints.size() - storageCap);
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
    QMetaObject::invokeMethod(m_worker, "setRetentionSeconds", Qt::QueuedConnection, Q_ARG(double, m_retentionSeconds));
    QMetaObject::invokeMethod(m_worker, "setMaxPointsPerSeries", Qt::QueuedConnection, Q_ARG(int, m_maxPointsPerSeries));
    QMetaObject::invokeMethod(m_worker, "setDisplayPointBudget", Qt::QueuedConnection, Q_ARG(int, m_displayPointBudget));
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

double PlotController::bufferMinX() const
{
    return m_bufferMinX;
}

double PlotController::bufferMaxX() const
{
    return m_bufferMaxX;
}

double PlotController::minSampleSpacing() const
{
    return m_minSampleSpacing;
}

double PlotController::windowSeconds() const
{
    return m_windowSeconds;
}

double PlotController::renderWindowSeconds() const
{
    return m_renderWindowSeconds;
}

double PlotController::retentionSeconds() const
{
    return m_retentionSeconds;
}

int PlotController::maxPointsPerSeries() const
{
    return m_maxPointsPerSeries;
}

int PlotController::displayPointBudget() const
{
    return m_displayPointBudget;
}

int PlotController::targetFps() const
{
    return m_targetFps;
}

bool PlotController::renderingEnabled() const
{
    return m_renderingEnabled;
}

double PlotController::inputSamplesPerSecond() const
{
    return m_inputSamplesPerSecond;
}

double PlotController::outputPointsPerSecond() const
{
    return m_outputPointsPerSecond;
}

double PlotController::renderFramesPerSecond() const
{
    return m_renderFramesPerSecond;
}

double PlotController::lastFrameBuildMs() const
{
    return m_lastFrameBuildMs;
}

double PlotController::avgFrameBuildMs() const
{
    return m_avgFrameBuildMs;
}

int PlotController::displayedPointCount() const
{
    return m_displayedPointCount;
}

double PlotController::decimationRatio() const
{
    return m_decimationRatio;
}

int PlotController::coalescedFrameRequests() const
{
    return m_coalescedFrameRequests;
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
    m_xMin = 0.0;
    m_xMax = qMax(0.01, m_windowSeconds);
    m_yMin = -1.0;
    m_yMax = 1.0;
    m_bufferMinX = 0.0;
    m_bufferMaxX = 0.0;
    m_minSampleSpacing = 0.001;
    emit axesChanged();
    emit bufferRangeChanged();

    QMetaObject::invokeMethod(m_worker, "clear", Qt::QueuedConnection);
}

void PlotController::requestRefresh()
{
    QMetaObject::invokeMethod(m_worker, "requestFrame", Qt::QueuedConnection);
}

void PlotController::requestViewFrame(double xMin, double xMax)
{
    QMetaObject::invokeMethod(m_worker,
                              "requestViewFrame",
                              Qt::QueuedConnection,
                              Q_ARG(double, xMin),
                              Q_ARG(double, xMax));
}

void PlotController::clearViewFrame()
{
    QMetaObject::invokeMethod(m_worker, "clearViewFrame", Qt::QueuedConnection);
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
    const double clamped = qBound(0.01, seconds, 3600.0);
    if (qFuzzyCompare(m_windowSeconds + 1.0, clamped + 1.0)) {
        return;
    }

    m_windowSeconds = clamped;
    emit windowSecondsChanged();

    QMetaObject::invokeMethod(m_worker, "setWindowSeconds", Qt::QueuedConnection, Q_ARG(double, m_windowSeconds));
}

void PlotController::setRenderWindowSeconds(double seconds)
{
    const double clamped = qBound(0.01, seconds, 120.0);
    if (qFuzzyCompare(m_renderWindowSeconds + 1.0, clamped + 1.0)) {
        return;
    }

    m_renderWindowSeconds = clamped;
    emit renderWindowSecondsChanged();

    QMetaObject::invokeMethod(m_worker, "setRenderWindowSeconds", Qt::QueuedConnection, Q_ARG(double, m_renderWindowSeconds));
}

void PlotController::setRetentionSeconds(double seconds)
{
    const double clamped = qBound(5.0, seconds, 3600.0);
    if (qFuzzyCompare(m_retentionSeconds + 1.0, clamped + 1.0)) {
        return;
    }

    m_retentionSeconds = clamped;
    emit retentionSecondsChanged();

    QMetaObject::invokeMethod(m_worker, "setRetentionSeconds", Qt::QueuedConnection, Q_ARG(double, m_retentionSeconds));
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

void PlotController::setDisplayPointBudget(int points)
{
    const int clamped = qBound(200, points, 40000);
    if (m_displayPointBudget == clamped) {
        return;
    }

    m_displayPointBudget = clamped;
    emit displayPointBudgetChanged();
    QMetaObject::invokeMethod(m_worker, "setDisplayPointBudget", Qt::QueuedConnection, Q_ARG(int, m_displayPointBudget));
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

void PlotController::onFrameReady(const QVariantList &seriesFrames,
                                  double xMin,
                                  double xMax,
                                  double yMin,
                                  double yMax,
                                  double bufferMinX,
                                  double bufferMaxX,
                                  double minSampleSpacing,
                                  double inputSamplesPerSecond,
                                  double outputPointsPerSecond,
                                  double renderFramesPerSecond,
                                  double lastFrameBuildMs,
                                  double avgFrameBuildMs,
                                  int displayedPointCount,
                                  double decimationRatio,
                                  int coalescedFrameRequests)
{
    m_xMin = xMin;
    m_xMax = xMax;
    m_yMin = yMin;
    m_yMax = yMax;
    emit axesChanged();

    const bool bufferChanged = !qFuzzyCompare(m_bufferMinX + 1.0, bufferMinX + 1.0)
        || !qFuzzyCompare(m_bufferMaxX + 1.0, bufferMaxX + 1.0)
        || !qFuzzyCompare(m_minSampleSpacing + 1.0, minSampleSpacing + 1.0);
    if (bufferChanged) {
        m_bufferMinX = bufferMinX;
        m_bufferMaxX = bufferMaxX;
        m_minSampleSpacing = qMax(0.000001, minSampleSpacing);
        emit bufferRangeChanged();
    }

    const bool telemetryUpdated = !qFuzzyCompare(m_inputSamplesPerSecond + 1.0, inputSamplesPerSecond + 1.0)
        || !qFuzzyCompare(m_outputPointsPerSecond + 1.0, outputPointsPerSecond + 1.0)
        || !qFuzzyCompare(m_renderFramesPerSecond + 1.0, renderFramesPerSecond + 1.0)
        || !qFuzzyCompare(m_lastFrameBuildMs + 1.0, lastFrameBuildMs + 1.0)
        || !qFuzzyCompare(m_avgFrameBuildMs + 1.0, avgFrameBuildMs + 1.0)
        || m_displayedPointCount != displayedPointCount
        || !qFuzzyCompare(m_decimationRatio + 1.0, decimationRatio + 1.0)
        || m_coalescedFrameRequests != coalescedFrameRequests;
    if (telemetryUpdated) {
        m_inputSamplesPerSecond = inputSamplesPerSecond;
        m_outputPointsPerSecond = outputPointsPerSecond;
        m_renderFramesPerSecond = renderFramesPerSecond;
        m_lastFrameBuildMs = lastFrameBuildMs;
        m_avgFrameBuildMs = avgFrameBuildMs;
        m_displayedPointCount = displayedPointCount;
        m_decimationRatio = decimationRatio;
        m_coalescedFrameRequests = coalescedFrameRequests;
        emit telemetryChanged();
    }

    for (const QVariant &seriesVariant : seriesFrames) {
        const QVariantMap packed = seriesVariant.toMap();
        const int address = packed.value(QStringLiteral("address")).toInt();
        const QString name = packed.value(QStringLiteral("name")).toString();
        const QVariantList points = packed.value(QStringLiteral("points")).toList();
        emit seriesFrame(address, name, points);
    }

    emit frameCompleted();
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
