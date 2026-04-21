/****************************************************************************
** Meta object code from reading C++ file 'plotcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../sicstudio/plotcontroller.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plotcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10PlotWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto PlotWorker::qt_create_metaobjectdata<qt_meta_tag_ZN10PlotWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PlotWorker",
        "frameReady",
        "",
        "QVariantList",
        "seriesFrames",
        "xMin",
        "xMax",
        "yMin",
        "yMax",
        "bufferMinX",
        "bufferMaxX",
        "minSampleSpacing",
        "inputSamplesPerSecond",
        "outputPointsPerSecond",
        "renderFramesPerSecond",
        "lastFrameBuildMs",
        "avgFrameBuildMs",
        "displayedPointCount",
        "decimationRatio",
        "coalescedFrameRequests",
        "cleared",
        "initialize",
        "enqueueSample",
        "address",
        "name",
        "tSec",
        "value",
        "enqueueSamples",
        "QList<PlotSample>",
        "samples",
        "requestFrame",
        "requestViewFrame",
        "clearViewFrame",
        "setTrackedSeries",
        "series",
        "setWindowSeconds",
        "seconds",
        "setRenderWindowSeconds",
        "setRetentionSeconds",
        "setMaxPointsPerSeries",
        "maxPoints",
        "setDisplayPointBudget",
        "points",
        "setTargetFps",
        "fps",
        "setRenderingEnabled",
        "enabled",
        "clear",
        "publishFrame"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'frameReady'
        QtMocHelpers::SignalData<void(const QVariantList &, double, double, double, double, double, double, double, double, double, double, double, double, int, double, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Double, 5 }, { QMetaType::Double, 6 }, { QMetaType::Double, 7 },
            { QMetaType::Double, 8 }, { QMetaType::Double, 9 }, { QMetaType::Double, 10 }, { QMetaType::Double, 11 },
            { QMetaType::Double, 12 }, { QMetaType::Double, 13 }, { QMetaType::Double, 14 }, { QMetaType::Double, 15 },
            { QMetaType::Double, 16 }, { QMetaType::Int, 17 }, { QMetaType::Double, 18 }, { QMetaType::Int, 19 },
        }}),
        // Signal 'cleared'
        QtMocHelpers::SignalData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'initialize'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'enqueueSample'
        QtMocHelpers::SlotData<void(int, const QString &, double, double)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 23 }, { QMetaType::QString, 24 }, { QMetaType::Double, 25 }, { QMetaType::Double, 26 },
        }}),
        // Slot 'enqueueSamples'
        QtMocHelpers::SlotData<void(const QVector<PlotSample> &)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 28, 29 },
        }}),
        // Slot 'requestFrame'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'requestViewFrame'
        QtMocHelpers::SlotData<void(double, double)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 5 }, { QMetaType::Double, 6 },
        }}),
        // Slot 'clearViewFrame'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setTrackedSeries'
        QtMocHelpers::SlotData<void(const QVariantList &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 34 },
        }}),
        // Slot 'setWindowSeconds'
        QtMocHelpers::SlotData<void(double)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 36 },
        }}),
        // Slot 'setRenderWindowSeconds'
        QtMocHelpers::SlotData<void(double)>(37, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 36 },
        }}),
        // Slot 'setRetentionSeconds'
        QtMocHelpers::SlotData<void(double)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 36 },
        }}),
        // Slot 'setMaxPointsPerSeries'
        QtMocHelpers::SlotData<void(int)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 40 },
        }}),
        // Slot 'setDisplayPointBudget'
        QtMocHelpers::SlotData<void(int)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 42 },
        }}),
        // Slot 'setTargetFps'
        QtMocHelpers::SlotData<void(int)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 44 },
        }}),
        // Slot 'setRenderingEnabled'
        QtMocHelpers::SlotData<void(bool)>(45, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 46 },
        }}),
        // Slot 'clear'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'publishFrame'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PlotWorker, qt_meta_tag_ZN10PlotWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PlotWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10PlotWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10PlotWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10PlotWorkerE_t>.metaTypes,
    nullptr
} };

void PlotWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PlotWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[8])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[9])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[10])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[11])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[12])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[13])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[14])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[15])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[16]))); break;
        case 1: _t->cleared(); break;
        case 2: _t->initialize(); break;
        case 3: _t->enqueueSample((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4]))); break;
        case 4: _t->enqueueSamples((*reinterpret_cast<std::add_pointer_t<QList<PlotSample>>>(_a[1]))); break;
        case 5: _t->requestFrame(); break;
        case 6: _t->requestViewFrame((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 7: _t->clearViewFrame(); break;
        case 8: _t->setTrackedSeries((*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[1]))); break;
        case 9: _t->setWindowSeconds((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 10: _t->setRenderWindowSeconds((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 11: _t->setRetentionSeconds((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 12: _t->setMaxPointsPerSeries((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->setDisplayPointBudget((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->setTargetFps((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->setRenderingEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 16: _t->clear(); break;
        case 17: _t->publishFrame(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<PlotSample> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PlotWorker::*)(const QVariantList & , double , double , double , double , double , double , double , double , double , double , double , double , int , double , int )>(_a, &PlotWorker::frameReady, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotWorker::*)()>(_a, &PlotWorker::cleared, 1))
            return;
    }
}

const QMetaObject *PlotWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlotWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10PlotWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PlotWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void PlotWorker::frameReady(const QVariantList & _t1, double _t2, double _t3, double _t4, double _t5, double _t6, double _t7, double _t8, double _t9, double _t10, double _t11, double _t12, double _t13, int _t14, double _t15, int _t16)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9, _t10, _t11, _t12, _t13, _t14, _t15, _t16);
}

// SIGNAL 1
void PlotWorker::cleared()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
namespace {
struct qt_meta_tag_ZN14PlotControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto PlotController::qt_create_metaobjectdata<qt_meta_tag_ZN14PlotControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PlotController",
        "seriesFrame",
        "",
        "address",
        "name",
        "QVariantList",
        "points",
        "axesChanged",
        "bufferRangeChanged",
        "windowSecondsChanged",
        "renderWindowSecondsChanged",
        "retentionSecondsChanged",
        "maxPointsPerSeriesChanged",
        "displayPointBudgetChanged",
        "targetFpsChanged",
        "renderingEnabledChanged",
        "telemetryChanged",
        "cleared",
        "frameCompleted",
        "ingestSample",
        "tSec",
        "value",
        "ingestSamples",
        "QList<PlotSample>",
        "samples",
        "setWindowSeconds",
        "seconds",
        "setRenderWindowSeconds",
        "setRetentionSeconds",
        "setMaxPointsPerSeries",
        "maxPoints",
        "setDisplayPointBudget",
        "setTargetFps",
        "fps",
        "setRenderingEnabled",
        "enabled",
        "onFrameReady",
        "seriesFrames",
        "xMin",
        "xMax",
        "yMin",
        "yMax",
        "bufferMinX",
        "bufferMaxX",
        "minSampleSpacing",
        "inputSamplesPerSecond",
        "outputPointsPerSecond",
        "renderFramesPerSecond",
        "lastFrameBuildMs",
        "avgFrameBuildMs",
        "displayedPointCount",
        "decimationRatio",
        "coalescedFrameRequests",
        "setSeriesEnabled",
        "clear",
        "syncTargetFpsToPrimaryScreen",
        "requestRefresh",
        "requestViewFrame",
        "clearViewFrame",
        "windowSeconds",
        "renderWindowSeconds",
        "retentionSeconds",
        "maxPointsPerSeries",
        "displayPointBudget",
        "targetFps",
        "renderingEnabled"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'seriesFrame'
        QtMocHelpers::SignalData<void(int, const QString &, const QVariantList &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 4 }, { 0x80000000 | 5, 6 },
        }}),
        // Signal 'axesChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'bufferRangeChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'windowSecondsChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'renderWindowSecondsChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'retentionSecondsChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'maxPointsPerSeriesChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'displayPointBudgetChanged'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'targetFpsChanged'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'renderingEnabledChanged'
        QtMocHelpers::SignalData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'telemetryChanged'
        QtMocHelpers::SignalData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cleared'
        QtMocHelpers::SignalData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'frameCompleted'
        QtMocHelpers::SignalData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'ingestSample'
        QtMocHelpers::SlotData<void(int, const QString &, double, double)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 4 }, { QMetaType::Double, 20 }, { QMetaType::Double, 21 },
        }}),
        // Slot 'ingestSamples'
        QtMocHelpers::SlotData<void(const QVector<PlotSample> &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 23, 24 },
        }}),
        // Slot 'setWindowSeconds'
        QtMocHelpers::SlotData<void(double)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 26 },
        }}),
        // Slot 'setRenderWindowSeconds'
        QtMocHelpers::SlotData<void(double)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 26 },
        }}),
        // Slot 'setRetentionSeconds'
        QtMocHelpers::SlotData<void(double)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 26 },
        }}),
        // Slot 'setMaxPointsPerSeries'
        QtMocHelpers::SlotData<void(int)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 30 },
        }}),
        // Slot 'setDisplayPointBudget'
        QtMocHelpers::SlotData<void(int)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'setTargetFps'
        QtMocHelpers::SlotData<void(int)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'setRenderingEnabled'
        QtMocHelpers::SlotData<void(bool)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 35 },
        }}),
        // Slot 'onFrameReady'
        QtMocHelpers::SlotData<void(const QVariantList &, double, double, double, double, double, double, double, double, double, double, double, double, int, double, int)>(36, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 5, 37 }, { QMetaType::Double, 38 }, { QMetaType::Double, 39 }, { QMetaType::Double, 40 },
            { QMetaType::Double, 41 }, { QMetaType::Double, 42 }, { QMetaType::Double, 43 }, { QMetaType::Double, 44 },
            { QMetaType::Double, 45 }, { QMetaType::Double, 46 }, { QMetaType::Double, 47 }, { QMetaType::Double, 48 },
            { QMetaType::Double, 49 }, { QMetaType::Int, 50 }, { QMetaType::Double, 51 }, { QMetaType::Int, 52 },
        }}),
        // Method 'setSeriesEnabled'
        QtMocHelpers::MethodData<void(int, const QString &, bool)>(53, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 4 }, { QMetaType::Bool, 35 },
        }}),
        // Method 'clear'
        QtMocHelpers::MethodData<void()>(54, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'syncTargetFpsToPrimaryScreen'
        QtMocHelpers::MethodData<void()>(55, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'requestRefresh'
        QtMocHelpers::MethodData<void()>(56, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'requestViewFrame'
        QtMocHelpers::MethodData<void(double, double)>(57, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 38 }, { QMetaType::Double, 39 },
        }}),
        // Method 'clearViewFrame'
        QtMocHelpers::MethodData<void()>(58, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'xMin'
        QtMocHelpers::PropertyData<double>(38, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'xMax'
        QtMocHelpers::PropertyData<double>(39, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'yMin'
        QtMocHelpers::PropertyData<double>(40, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'yMax'
        QtMocHelpers::PropertyData<double>(41, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'bufferMinX'
        QtMocHelpers::PropertyData<double>(42, QMetaType::Double, QMC::DefaultPropertyFlags, 2),
        // property 'bufferMaxX'
        QtMocHelpers::PropertyData<double>(43, QMetaType::Double, QMC::DefaultPropertyFlags, 2),
        // property 'minSampleSpacing'
        QtMocHelpers::PropertyData<double>(44, QMetaType::Double, QMC::DefaultPropertyFlags, 2),
        // property 'windowSeconds'
        QtMocHelpers::PropertyData<double>(59, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'renderWindowSeconds'
        QtMocHelpers::PropertyData<double>(60, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'retentionSeconds'
        QtMocHelpers::PropertyData<double>(61, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'maxPointsPerSeries'
        QtMocHelpers::PropertyData<int>(62, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 6),
        // property 'displayPointBudget'
        QtMocHelpers::PropertyData<int>(63, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 7),
        // property 'targetFps'
        QtMocHelpers::PropertyData<int>(64, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 8),
        // property 'renderingEnabled'
        QtMocHelpers::PropertyData<bool>(65, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 9),
        // property 'inputSamplesPerSecond'
        QtMocHelpers::PropertyData<double>(45, QMetaType::Double, QMC::DefaultPropertyFlags, 10),
        // property 'outputPointsPerSecond'
        QtMocHelpers::PropertyData<double>(46, QMetaType::Double, QMC::DefaultPropertyFlags, 10),
        // property 'renderFramesPerSecond'
        QtMocHelpers::PropertyData<double>(47, QMetaType::Double, QMC::DefaultPropertyFlags, 10),
        // property 'lastFrameBuildMs'
        QtMocHelpers::PropertyData<double>(48, QMetaType::Double, QMC::DefaultPropertyFlags, 10),
        // property 'avgFrameBuildMs'
        QtMocHelpers::PropertyData<double>(49, QMetaType::Double, QMC::DefaultPropertyFlags, 10),
        // property 'displayedPointCount'
        QtMocHelpers::PropertyData<int>(50, QMetaType::Int, QMC::DefaultPropertyFlags, 10),
        // property 'decimationRatio'
        QtMocHelpers::PropertyData<double>(51, QMetaType::Double, QMC::DefaultPropertyFlags, 10),
        // property 'coalescedFrameRequests'
        QtMocHelpers::PropertyData<int>(52, QMetaType::Int, QMC::DefaultPropertyFlags, 10),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PlotController, qt_meta_tag_ZN14PlotControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PlotController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14PlotControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14PlotControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14PlotControllerE_t>.metaTypes,
    nullptr
} };

void PlotController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PlotController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->seriesFrame((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[3]))); break;
        case 1: _t->axesChanged(); break;
        case 2: _t->bufferRangeChanged(); break;
        case 3: _t->windowSecondsChanged(); break;
        case 4: _t->renderWindowSecondsChanged(); break;
        case 5: _t->retentionSecondsChanged(); break;
        case 6: _t->maxPointsPerSeriesChanged(); break;
        case 7: _t->displayPointBudgetChanged(); break;
        case 8: _t->targetFpsChanged(); break;
        case 9: _t->renderingEnabledChanged(); break;
        case 10: _t->telemetryChanged(); break;
        case 11: _t->cleared(); break;
        case 12: _t->frameCompleted(); break;
        case 13: _t->ingestSample((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4]))); break;
        case 14: _t->ingestSamples((*reinterpret_cast<std::add_pointer_t<QList<PlotSample>>>(_a[1]))); break;
        case 15: _t->setWindowSeconds((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 16: _t->setRenderWindowSeconds((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 17: _t->setRetentionSeconds((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 18: _t->setMaxPointsPerSeries((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->setDisplayPointBudget((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->setTargetFps((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->setRenderingEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 22: _t->onFrameReady((*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[8])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[9])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[10])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[11])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[12])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[13])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[14])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[15])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[16]))); break;
        case 23: _t->setSeriesEnabled((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 24: _t->clear(); break;
        case 25: _t->syncTargetFpsToPrimaryScreen(); break;
        case 26: _t->requestRefresh(); break;
        case 27: _t->requestViewFrame((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 28: _t->clearViewFrame(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<PlotSample> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)(int , const QString & , const QVariantList & )>(_a, &PlotController::seriesFrame, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::axesChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::bufferRangeChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::windowSecondsChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::renderWindowSecondsChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::retentionSecondsChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::maxPointsPerSeriesChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::displayPointBudgetChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::targetFpsChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::renderingEnabledChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::telemetryChanged, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::cleared, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::frameCompleted, 12))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->xMin(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->xMax(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->yMin(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->yMax(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->bufferMinX(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->bufferMaxX(); break;
        case 6: *reinterpret_cast<double*>(_v) = _t->minSampleSpacing(); break;
        case 7: *reinterpret_cast<double*>(_v) = _t->windowSeconds(); break;
        case 8: *reinterpret_cast<double*>(_v) = _t->renderWindowSeconds(); break;
        case 9: *reinterpret_cast<double*>(_v) = _t->retentionSeconds(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->maxPointsPerSeries(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->displayPointBudget(); break;
        case 12: *reinterpret_cast<int*>(_v) = _t->targetFps(); break;
        case 13: *reinterpret_cast<bool*>(_v) = _t->renderingEnabled(); break;
        case 14: *reinterpret_cast<double*>(_v) = _t->inputSamplesPerSecond(); break;
        case 15: *reinterpret_cast<double*>(_v) = _t->outputPointsPerSecond(); break;
        case 16: *reinterpret_cast<double*>(_v) = _t->renderFramesPerSecond(); break;
        case 17: *reinterpret_cast<double*>(_v) = _t->lastFrameBuildMs(); break;
        case 18: *reinterpret_cast<double*>(_v) = _t->avgFrameBuildMs(); break;
        case 19: *reinterpret_cast<int*>(_v) = _t->displayedPointCount(); break;
        case 20: *reinterpret_cast<double*>(_v) = _t->decimationRatio(); break;
        case 21: *reinterpret_cast<int*>(_v) = _t->coalescedFrameRequests(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 7: _t->setWindowSeconds(*reinterpret_cast<double*>(_v)); break;
        case 8: _t->setRenderWindowSeconds(*reinterpret_cast<double*>(_v)); break;
        case 9: _t->setRetentionSeconds(*reinterpret_cast<double*>(_v)); break;
        case 10: _t->setMaxPointsPerSeries(*reinterpret_cast<int*>(_v)); break;
        case 11: _t->setDisplayPointBudget(*reinterpret_cast<int*>(_v)); break;
        case 12: _t->setTargetFps(*reinterpret_cast<int*>(_v)); break;
        case 13: _t->setRenderingEnabled(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *PlotController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlotController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14PlotControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PlotController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void PlotController::seriesFrame(int _t1, const QString & _t2, const QVariantList & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}

// SIGNAL 1
void PlotController::axesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void PlotController::bufferRangeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PlotController::windowSecondsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void PlotController::renderWindowSecondsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void PlotController::retentionSecondsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void PlotController::maxPointsPerSeriesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void PlotController::displayPointBudgetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void PlotController::targetFpsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void PlotController::renderingEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void PlotController::telemetryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void PlotController::cleared()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void PlotController::frameCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
QT_WARNING_POP
