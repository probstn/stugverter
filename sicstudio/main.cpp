#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include <QMetaType>

#include "dictionarymodel.h"
#include "inverterclient.h"
#include "plotcontroller.h"
#include "plotsample.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qRegisterMetaType<PlotSample>("PlotSample");
    qRegisterMetaType<QVector<PlotSample>>("QVector<PlotSample>");

    DictionaryModel dictionaryModel;
    InverterClient inverterClient(&dictionaryModel);
    PlotController plotController;

    QObject::connect(&inverterClient,
                     &InverterClient::streamSample,
                     &plotController,
                     &PlotController::ingestSample,
                     Qt::QueuedConnection);
    QObject::connect(&inverterClient,
                     &InverterClient::streamSamplesReady,
                     &plotController,
                     &PlotController::ingestSamples,
                     Qt::QueuedConnection);
    QObject::connect(&inverterClient,
                     &InverterClient::dictionaryReloaded,
                     &plotController,
                     &PlotController::clear,
                     Qt::QueuedConnection);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dictionaryModel", &dictionaryModel);
    engine.rootContext()->setContextProperty("inverterClient", &inverterClient);
    engine.rootContext()->setContextProperty("plotController", &plotController);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SICStudio", "Main");

    return QCoreApplication::exec();
}
