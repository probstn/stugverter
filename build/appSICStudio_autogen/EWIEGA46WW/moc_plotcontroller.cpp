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
        "setTrackedSeries",
        "series",
        "setWindowSeconds",
        "seconds",
        "setRenderWindowSeconds",
        "setMaxPointsPerSeries",
        "maxPoints",
        "setTargetFps",
        "fps",
        "setRenderingEnabled",
        "enabled",
        "clear",
        "publishFrame"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'frameReady'
        QtMocHelpers::SignalData<void(const QVariantList &, double, double, double, double)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Double, 5 }, { QMetaType::Double, 6 }, { QMetaType::Double, 7 },
            { QMetaType::Double, 8 },
        }}),
        // Signal 'cleared'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'initialize'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'enqueueSample'
        QtMocHelpers::SlotData<void(int, const QString &, double, double)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 }, { QMetaType::QString, 13 }, { QMetaType::Double, 14 }, { QMetaType::Double, 15 },
        }}),
        // Slot 'enqueueSamples'
        QtMocHelpers::SlotData<void(const QVector<PlotSample> &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 17, 18 },
        }}),
        // Slot 'setTrackedSeries'
        QtMocHelpers::SlotData<void(const QVariantList &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 20 },
        }}),
        // Slot 'setWindowSeconds'
        QtMocHelpers::SlotData<void(double)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 22 },
        }}),
        // Slot 'setRenderWindowSeconds'
        QtMocHelpers::SlotData<void(double)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 22 },
        }}),
        // Slot 'setMaxPointsPerSeries'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 25 },
        }}),
        // Slot 'setTargetFps'
        QtMocHelpers::SlotData<void(int)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 27 },
        }}),
        // Slot 'setRenderingEnabled'
        QtMocHelpers::SlotData<void(bool)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 29 },
        }}),
        // Slot 'clear'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'publishFrame'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
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
        case 0: _t->frameReady((*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5]))); break;
        case 1: _t->cleared(); break;
        case 2: _t->initialize(); break;
        case 3: _t->enqueueSample((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4]))); break;
        case 4: _t->enqueueSamples((*reinterpret_cast<std::add_pointer_t<QList<PlotSample>>>(_a[1]))); break;
        case 5: _t->setTrackedSeries((*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[1]))); break;
        case 6: _t->setWindowSeconds((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 7: _t->setRenderWindowSeconds((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 8: _t->setMaxPointsPerSeries((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->setTargetFps((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->setRenderingEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 11: _t->clear(); break;
        case 12: _t->publishFrame(); break;
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
        if (QtMocHelpers::indexOfMethod<void (PlotWorker::*)(const QVariantList & , double , double , double , double )>(_a, &PlotWorker::frameReady, 0))
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
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void PlotWorker::frameReady(const QVariantList & _t1, double _t2, double _t3, double _t4, double _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5);
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
        "windowSecondsChanged",
        "renderWindowSecondsChanged",
        "maxPointsPerSeriesChanged",
        "targetFpsChanged",
        "renderingEnabledChanged",
        "cleared",
        "ingestSample",
        "tSec",
        "value",
        "ingestSamples",
        "QList<PlotSample>",
        "samples",
        "setWindowSeconds",
        "seconds",
        "setRenderWindowSeconds",
        "setMaxPointsPerSeries",
        "maxPoints",
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
        "setSeriesEnabled",
        "clear",
        "syncTargetFpsToPrimaryScreen",
        "windowSeconds",
        "renderWindowSeconds",
        "maxPointsPerSeries",
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
        // Signal 'windowSecondsChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'renderWindowSecondsChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'maxPointsPerSeriesChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'targetFpsChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'renderingEnabledChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cleared'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'ingestSample'
        QtMocHelpers::SlotData<void(int, const QString &, double, double)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 4 }, { QMetaType::Double, 15 }, { QMetaType::Double, 16 },
        }}),
        // Slot 'ingestSamples'
        QtMocHelpers::SlotData<void(const QVector<PlotSample> &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Slot 'setWindowSeconds'
        QtMocHelpers::SlotData<void(double)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 21 },
        }}),
        // Slot 'setRenderWindowSeconds'
        QtMocHelpers::SlotData<void(double)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 21 },
        }}),
        // Slot 'setMaxPointsPerSeries'
        QtMocHelpers::SlotData<void(int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 24 },
        }}),
        // Slot 'setTargetFps'
        QtMocHelpers::SlotData<void(int)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 26 },
        }}),
        // Slot 'setRenderingEnabled'
        QtMocHelpers::SlotData<void(bool)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 28 },
        }}),
        // Slot 'onFrameReady'
        QtMocHelpers::SlotData<void(const QVariantList &, double, double, double, double)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 5, 30 }, { QMetaType::Double, 31 }, { QMetaType::Double, 32 }, { QMetaType::Double, 33 },
            { QMetaType::Double, 34 },
        }}),
        // Method 'setSeriesEnabled'
        QtMocHelpers::MethodData<void(int, const QString &, bool)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 4 }, { QMetaType::Bool, 28 },
        }}),
        // Method 'clear'
        QtMocHelpers::MethodData<void()>(36, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'syncTargetFpsToPrimaryScreen'
        QtMocHelpers::MethodData<void()>(37, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'xMin'
        QtMocHelpers::PropertyData<double>(31, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'xMax'
        QtMocHelpers::PropertyData<double>(32, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'yMin'
        QtMocHelpers::PropertyData<double>(33, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'yMax'
        QtMocHelpers::PropertyData<double>(34, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'windowSeconds'
        QtMocHelpers::PropertyData<double>(38, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'renderWindowSeconds'
        QtMocHelpers::PropertyData<double>(39, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'maxPointsPerSeries'
        QtMocHelpers::PropertyData<int>(40, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'targetFps'
        QtMocHelpers::PropertyData<int>(41, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'renderingEnabled'
        QtMocHelpers::PropertyData<bool>(42, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 6),
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
        case 2: _t->windowSecondsChanged(); break;
        case 3: _t->renderWindowSecondsChanged(); break;
        case 4: _t->maxPointsPerSeriesChanged(); break;
        case 5: _t->targetFpsChanged(); break;
        case 6: _t->renderingEnabledChanged(); break;
        case 7: _t->cleared(); break;
        case 8: _t->ingestSample((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4]))); break;
        case 9: _t->ingestSamples((*reinterpret_cast<std::add_pointer_t<QList<PlotSample>>>(_a[1]))); break;
        case 10: _t->setWindowSeconds((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 11: _t->setRenderWindowSeconds((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 12: _t->setMaxPointsPerSeries((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->setTargetFps((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->setRenderingEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 15: _t->onFrameReady((*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5]))); break;
        case 16: _t->setSeriesEnabled((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 17: _t->clear(); break;
        case 18: _t->syncTargetFpsToPrimaryScreen(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 9:
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
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::windowSecondsChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::renderWindowSecondsChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::maxPointsPerSeriesChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::targetFpsChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::renderingEnabledChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (PlotController::*)()>(_a, &PlotController::cleared, 7))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->xMin(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->xMax(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->yMin(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->yMax(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->windowSeconds(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->renderWindowSeconds(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->maxPointsPerSeries(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->targetFps(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->renderingEnabled(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 4: _t->setWindowSeconds(*reinterpret_cast<double*>(_v)); break;
        case 5: _t->setRenderWindowSeconds(*reinterpret_cast<double*>(_v)); break;
        case 6: _t->setMaxPointsPerSeries(*reinterpret_cast<int*>(_v)); break;
        case 7: _t->setTargetFps(*reinterpret_cast<int*>(_v)); break;
        case 8: _t->setRenderingEnabled(*reinterpret_cast<bool*>(_v)); break;
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
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
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
void PlotController::windowSecondsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PlotController::renderWindowSecondsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void PlotController::maxPointsPerSeriesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void PlotController::targetFpsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void PlotController::renderingEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void PlotController::cleared()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
