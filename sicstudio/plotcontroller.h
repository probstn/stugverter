#ifndef PLOTCONTROLLER_H
#define PLOTCONTROLLER_H

#include <QHash>
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
    void setTrackedSeries(const QVariantList &series);
    void setWindowSeconds(double seconds);
    void setRenderWindowSeconds(double seconds);
    void setMaxPointsPerSeries(int maxPoints);
    void setTargetFps(int fps);
    void setRenderingEnabled(bool enabled);
    void clear();

signals:
    void frameReady(const QVariantList &seriesFrames, double xMin, double xMax, double yMin, double yMax);
    void cleared();

private slots:
    void publishFrame();

private:
    struct SeriesState {
        QString name;
        QVector<QPointF> points;
        bool dirty = false;
        bool initialized = false;
        double minY = 0.0;
        double maxY = 0.0;
        double latestX = 0.0;
        bool bucketActive = false;
        qint64 bucketIndex = -1;
        double bucketX = 0.0;
        double bucketLastX = 0.0;
        double bucketMin = 0.0;
        double bucketMax = 0.0;
        double bucketSum = 0.0;
        int bucketCount = 0;
    };

    QHash<int, SeriesState> m_series;
    QSet<int> m_tracked;
    QSet<int> m_dirtyAddresses;

    QTimer *m_frameTimer = nullptr;
    double m_windowSeconds = 10.0;
    double m_renderWindowSeconds = 10.0;
    int m_maxPointsPerSeries = 5000;
    int m_targetFps = 60;
    double m_lastX = 0.0;
    bool m_renderingEnabled = false;

    double bucketDuration() const;
    void pushBucketPoint(SeriesState &state, double x, double y);
    void flushBucket(SeriesState &state);
    void trimOldPoints(SeriesState &state, double keepFromX);
};

class PlotController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double xMin READ xMin NOTIFY axesChanged)
    Q_PROPERTY(double xMax READ xMax NOTIFY axesChanged)
    Q_PROPERTY(double yMin READ yMin NOTIFY axesChanged)
    Q_PROPERTY(double yMax READ yMax NOTIFY axesChanged)
    Q_PROPERTY(double windowSeconds READ windowSeconds WRITE setWindowSeconds NOTIFY windowSecondsChanged)
    Q_PROPERTY(double renderWindowSeconds READ renderWindowSeconds WRITE setRenderWindowSeconds NOTIFY renderWindowSecondsChanged)
    Q_PROPERTY(int maxPointsPerSeries READ maxPointsPerSeries WRITE setMaxPointsPerSeries NOTIFY maxPointsPerSeriesChanged)
    Q_PROPERTY(int targetFps READ targetFps WRITE setTargetFps NOTIFY targetFpsChanged)
    Q_PROPERTY(bool renderingEnabled READ renderingEnabled WRITE setRenderingEnabled NOTIFY renderingEnabledChanged)

public:
    explicit PlotController(QObject *parent = nullptr);
    ~PlotController() override;

    double xMin() const;
    double xMax() const;
    double yMin() const;
    double yMax() const;

    double windowSeconds() const;
    double renderWindowSeconds() const;
    int maxPointsPerSeries() const;
    int targetFps() const;
    bool renderingEnabled() const;

    Q_INVOKABLE void setSeriesEnabled(int address, const QString &name, bool enabled);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void syncTargetFpsToPrimaryScreen();

public slots:
    void ingestSample(int address, const QString &name, double tSec, double value);
    void ingestSamples(const QVector<PlotSample> &samples);
    void setWindowSeconds(double seconds);
    void setRenderWindowSeconds(double seconds);
    void setMaxPointsPerSeries(int maxPoints);
    void setTargetFps(int fps);
    void setRenderingEnabled(bool enabled);

signals:
    void seriesFrame(int address, const QString &name, const QVariantList &points);
    void axesChanged();
    void windowSecondsChanged();
    void renderWindowSecondsChanged();
    void maxPointsPerSeriesChanged();
    void targetFpsChanged();
    void renderingEnabledChanged();
    void cleared();

private slots:
    void onFrameReady(const QVariantList &seriesFrames, double xMin, double xMax, double yMin, double yMax);

private:
    QThread m_workerThread;
    PlotWorker *m_worker = nullptr;

    QHash<int, QString> m_enabledSeries;

    double m_xMin = 0.0;
    double m_xMax = 10.0;
    double m_yMin = -1.0;
    double m_yMax = 1.0;

    double m_windowSeconds = 10.0;
    double m_renderWindowSeconds = 10.0;
    int m_maxPointsPerSeries = 5000;
    int m_targetFps = 120;
    bool m_renderingEnabled = false;

    void pushTrackedSeriesToWorker();
};

#endif