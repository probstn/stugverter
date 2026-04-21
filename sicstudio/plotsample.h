#ifndef PLOTSAMPLE_H
#define PLOTSAMPLE_H

#include <QMetaType>
#include <QString>
#include <QVector>

struct PlotSample {
    int address = 0;
    QString name;
    double tSec = 0.0;
    double value = 0.0;
};

Q_DECLARE_METATYPE(PlotSample)
Q_DECLARE_METATYPE(QVector<PlotSample>)

#endif