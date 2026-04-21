/****************************************************************************
** Meta object code from reading C++ file 'Plotter.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../stuglab/Plotter.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Plotter.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7PlotterE_t {};
} // unnamed namespace

template <> constexpr inline auto Plotter::qt_create_metaobjectdata<qt_meta_tag_ZN7PlotterE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Plotter",
        "QML.Element",
        "auto",
        "targetSeriesChanged",
        "",
        "displayWindowChanged",
        "isPausedChanged",
        "panOffsetChanged",
        "maxPanOffsetChanged",
        "updatePlot",
        "addPoint",
        "value",
        "targetSeries",
        "QXYSeries*",
        "displayWindow",
        "isPaused",
        "panOffset",
        "maxPanOffset"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'targetSeriesChanged'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'displayWindowChanged'
        QtMocHelpers::SignalData<void()>(5, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isPausedChanged'
        QtMocHelpers::SignalData<void()>(6, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'panOffsetChanged'
        QtMocHelpers::SignalData<void()>(7, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'maxPanOffsetChanged'
        QtMocHelpers::SignalData<void()>(8, 4, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updatePlot'
        QtMocHelpers::SlotData<void()>(9, 4, QMC::AccessPrivate, QMetaType::Void),
        // Method 'addPoint'
        QtMocHelpers::MethodData<void(float)>(10, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 11 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'targetSeries'
        QtMocHelpers::PropertyData<QXYSeries*>(12, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 0),
        // property 'displayWindow'
        QtMocHelpers::PropertyData<int>(14, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'isPaused'
        QtMocHelpers::PropertyData<bool>(15, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'panOffset'
        QtMocHelpers::PropertyData<int>(16, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'maxPanOffset'
        QtMocHelpers::PropertyData<int>(17, QMetaType::Int, QMC::DefaultPropertyFlags, 4),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<Plotter, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject Plotter::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7PlotterE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7PlotterE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7PlotterE_t>.metaTypes,
    nullptr
} };

void Plotter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Plotter *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->targetSeriesChanged(); break;
        case 1: _t->displayWindowChanged(); break;
        case 2: _t->isPausedChanged(); break;
        case 3: _t->panOffsetChanged(); break;
        case 4: _t->maxPanOffsetChanged(); break;
        case 5: _t->updatePlot(); break;
        case 6: _t->addPoint((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Plotter::*)()>(_a, &Plotter::targetSeriesChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Plotter::*)()>(_a, &Plotter::displayWindowChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Plotter::*)()>(_a, &Plotter::isPausedChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Plotter::*)()>(_a, &Plotter::panOffsetChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Plotter::*)()>(_a, &Plotter::maxPanOffsetChanged, 4))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QXYSeries* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QXYSeries**>(_v) = _t->targetSeries(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->displayWindow(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isPaused(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->panOffset(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->maxPanOffset(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTargetSeries(*reinterpret_cast<QXYSeries**>(_v)); break;
        case 1: _t->setDisplayWindow(*reinterpret_cast<int*>(_v)); break;
        case 2: _t->setIsPaused(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setPanOffset(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Plotter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Plotter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7PlotterE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Plotter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Plotter::targetSeriesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Plotter::displayWindowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Plotter::isPausedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Plotter::panOffsetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Plotter::maxPanOffsetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
