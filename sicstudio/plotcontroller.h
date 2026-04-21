#ifndef PLOTCONTROLLER_H
#define PLOTCONTROLLER_H

#include <QHash>
#include <QElapsedTimer>
#include <QObject>
#include <QPointF>
#include <QSet>
#include <QThread>
#include <QTimer>
#include <QVariantList>
#include <QVector>

#include "plotsample.h"

class PlotWorker : public QObject
{
    Q_OBJECT

public:
    explicit PlotWorker(QObject *parent = nullptr);

public slots:
    void initialize();
    void enqueueSample(int address, const QString &name, double tSec, double value);
    void enqueueSamples(const QVector<PlotSample> &samples);
    void requestFrame();
    void requestViewFrame(double xMin, double xMax);
    void clearViewFrame();
    void setTrackedSeries(const QVariantList &series);
    void setWindowSeconds(double seconds);
    void setRenderWindowSeconds(double seconds);
    void setRetentionSeconds(double seconds);
    void setMaxPointsPerSeries(int maxPoints);
    void setDisplayPointBudget(int points);
    void setTargetFps(int fps);
    void setRenderingEnabled(bool enabled);
    void clear();

signals:
    void frameReady(const QVariantList &seriesFrames,
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
                    int coalescedFrameRequests);
    void cleared();

private slots:
    void publishFrame();

private:
    struct SeriesState {
        QString name;
        QVector<QPointF> rawPoints;
        bool dirty = false;
        double latestX = 0.0;
    };

    QHash<int, SeriesState> m_series;
    QSet<int> m_tracked;
    QSet<int> m_dirtyAddresses;

    QTimer *m_frameTimer = nullptr;
    double m_visibleWindowSeconds = 10.0;
    double m_windowSeconds = 10.0;
    double m_retentionSeconds = 120.0;
    int m_maxPointsPerSeries = 5000;
    int m_displayPointBudget = 3500;
    int m_targetFps = 60;
    double m_lastX = 0.0;
    bool m_renderingEnabled = false;
    bool m_hasManualView = false;
    double m_manualXMin = 0.0;
    double m_manualXMax = 10.0;
    bool m_framePublishQueued = false;
    bool m_forceFrameRequested = false;
    int m_coalescedFrameRequests = 0;

    QElapsedTimer m_telemetryClock;
    qint64 m_ingestedSamplesWindow = 0;
    qint64 m_outputPointsWindow = 0;
    int m_frameCountWindow = 0;
    double m_inputSamplesPerSecond = 0.0;
    double m_outputPointsPerSecond = 0.0;
    double m_renderFramesPerSecond = 0.0;
    double m_lastFrameBuildMs = 0.0;
    double m_avgFrameBuildMs = 0.0;
    int m_displayedPointCount = 0;
    double m_decimationRatio = 1.0;

    static constexpr int StorageOversampling = 12;
    static constexpr int SpacingProbeIntervals = 256;

    void trimOldPoints(SeriesState &state, double keepFromX);
    QVariantList buildDisplayPoints(const QVector<QPointF> &rawPoints,
                                    double xMin,
                                    double xMax,
                                    double *yMin,
                                    double *yMax,
                                    int *rawVisibleCount,
                                    int targetCount) const;
    void schedulePublishFrame();
};

class PlotController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double xMin READ xMin NOTIFY axesChanged)
    Q_PROPERTY(double xMax READ xMax NOTIFY axesChanged)
    Q_PROPERTY(double yMin READ yMin NOTIFY axesChanged)
    Q_PROPERTY(double yMax READ yMax NOTIFY axesChanged)
    Q_PROPERTY(double bufferMinX READ bufferMinX NOTIFY bufferRangeChanged)
    Q_PROPERTY(double bufferMaxX READ bufferMaxX NOTIFY bufferRangeChanged)
    Q_PROPERTY(double minSampleSpacing READ minSampleSpacing NOTIFY bufferRangeChanged)
    Q_PROPERTY(double windowSeconds READ windowSeconds WRITE setWindowSeconds NOTIFY windowSecondsChanged)
    Q_PROPERTY(double renderWindowSeconds READ renderWindowSeconds WRITE setRenderWindowSeconds NOTIFY renderWindowSecondsChanged)
    Q_PROPERTY(double retentionSeconds READ retentionSeconds WRITE setRetentionSeconds NOTIFY retentionSecondsChanged)
    Q_PROPERTY(int maxPointsPerSeries READ maxPointsPerSeries WRITE setMaxPointsPerSeries NOTIFY maxPointsPerSeriesChanged)
    Q_PROPERTY(int displayPointBudget READ displayPointBudget WRITE setDisplayPointBudget NOTIFY displayPointBudgetChanged)
    Q_PROPERTY(int targetFps READ targetFps WRITE setTargetFps NOTIFY targetFpsChanged)
    Q_PROPERTY(bool renderingEnabled READ renderingEnabled WRITE setRenderingEnabled NOTIFY renderingEnabledChanged)
    Q_PROPERTY(double inputSamplesPerSecond READ inputSamplesPerSecond NOTIFY telemetryChanged)
    Q_PROPERTY(double outputPointsPerSecond READ outputPointsPerSecond NOTIFY telemetryChanged)
    Q_PROPERTY(double renderFramesPerSecond READ renderFramesPerSecond NOTIFY telemetryChanged)
    Q_PROPERTY(double lastFrameBuildMs READ lastFrameBuildMs NOTIFY telemetryChanged)
    Q_PROPERTY(double avgFrameBuildMs READ avgFrameBuildMs NOTIFY telemetryChanged)
    Q_PROPERTY(int displayedPointCount READ displayedPointCount NOTIFY telemetryChanged)
    Q_PROPERTY(double decimationRatio READ decimationRatio NOTIFY telemetryChanged)
    Q_PROPERTY(int coalescedFrameRequests READ coalescedFrameRequests NOTIFY telemetryChanged)

public:
    explicit PlotController(QObject *parent = nullptr);
    ~PlotController() override;

    double xMin() const;
    double xMax() const;
    double yMin() const;
    double yMax() const;
    double bufferMinX() const;
    double bufferMaxX() const;
    double minSampleSpacing() const;

    double windowSeconds() const;
    double renderWindowSeconds() const;
    double retentionSeconds() const;
    int maxPointsPerSeries() const;
    int displayPointBudget() const;
    int targetFps() const;
    bool renderingEnabled() const;
    double inputSamplesPerSecond() const;
    double outputPointsPerSecond() const;
    double renderFramesPerSecond() const;
    double lastFrameBuildMs() const;
    double avgFrameBuildMs() const;
    int displayedPointCount() const;
    double decimationRatio() const;
    int coalescedFrameRequests() const;

    Q_INVOKABLE void setSeriesEnabled(int address, const QString &name, bool enabled);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void syncTargetFpsToPrimaryScreen();
    Q_INVOKABLE void requestRefresh();
    Q_INVOKABLE void requestViewFrame(double xMin, double xMax);
    Q_INVOKABLE void clearViewFrame();

public slots:
    void ingestSample(int address, const QString &name, double tSec, double value);
    void ingestSamples(const QVector<PlotSample> &samples);
    void setWindowSeconds(double seconds);
    void setRenderWindowSeconds(double seconds);
    void setRetentionSeconds(double seconds);
    void setMaxPointsPerSeries(int maxPoints);
    void setDisplayPointBudget(int points);
    void setTargetFps(int fps);
    void setRenderingEnabled(bool enabled);

signals:
    void seriesFrame(int address, const QString &name, const QVariantList &points);
    void axesChanged();
    void bufferRangeChanged();
    void windowSecondsChanged();
    void renderWindowSecondsChanged();
    void retentionSecondsChanged();
    void maxPointsPerSeriesChanged();
    void displayPointBudgetChanged();
    void targetFpsChanged();
    void renderingEnabledChanged();
    void telemetryChanged();
    void cleared();
    void frameCompleted();

private slots:
    void onFrameReady(const QVariantList &seriesFrames,
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
                      int coalescedFrameRequests);

private:
    QThread m_workerThread;
    PlotWorker *m_worker = nullptr;

    QHash<int, QString> m_enabledSeries;

    double m_xMin = 0.0;
    double m_xMax = 10.0;
    double m_yMin = -1.0;
    double m_yMax = 1.0;
    double m_bufferMinX = 0.0;
    double m_bufferMaxX = 0.0;
    double m_minSampleSpacing = 0.001;

    double m_windowSeconds = 10.0;
    double m_renderWindowSeconds = 10.0;
    double m_retentionSeconds = 120.0;
    int m_maxPointsPerSeries = 5000;
    int m_displayPointBudget = 3500;
    int m_targetFps = 120;
    bool m_renderingEnabled = false;

    double m_inputSamplesPerSecond = 0.0;
    double m_outputPointsPerSecond = 0.0;
    double m_renderFramesPerSecond = 0.0;
    double m_lastFrameBuildMs = 0.0;
    double m_avgFrameBuildMs = 0.0;
    int m_displayedPointCount = 0;
    double m_decimationRatio = 1.0;
    int m_coalescedFrameRequests = 0;

    void pushTrackedSeriesToWorker();
};

#endif