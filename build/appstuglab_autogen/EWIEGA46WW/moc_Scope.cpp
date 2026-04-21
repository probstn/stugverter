/****************************************************************************
** Meta object code from reading C++ file 'Scope.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../stuglab/Scope.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Scope.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN5ScopeE_t {};
} // unnamed namespace

template <> constexpr inline auto Scope::qt_create_metaobjectdata<qt_meta_tag_ZN5ScopeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Scope",
        "QML.Element",
        "auto",
        "QML.Creatable",
        "false",
        "QML.UncreatableReason",
        "Scopes are instantiated and managed by ScopeManager",
        "nameChanged",
        "",
        "runningChanged",
        "addressesChanged",
        "addressAdded",
        "address",
        "addressRemoved",
        "streamControlRequested",
        "streamId",
        "start",
        "pointsCountChanged",
        "addAddress",
        "dtype",
        "removeAddress",
        "getYMin",
        "getYMax",
        "setYMin",
        "min",
        "setYMax",
        "max",
        "play",
        "pause",
        "updateGraphSeries",
        "QXYSeries*",
        "series",
        "bufferSize",
        "name",
        "frequency",
        "running",
        "addresses",
        "QList<quint16>",
        "pointsCount"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'nameChanged'
        QtMocHelpers::SignalData<void()>(7, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'runningChanged'
        QtMocHelpers::SignalData<void()>(9, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'addressesChanged'
        QtMocHelpers::SignalData<void()>(10, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'addressAdded'
        QtMocHelpers::SignalData<void(quint16)>(11, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UShort, 12 },
        }}),
        // Signal 'addressRemoved'
        QtMocHelpers::SignalData<void(quint16)>(13, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UShort, 12 },
        }}),
        // Signal 'streamControlRequested'
        QtMocHelpers::SignalData<void(quint8, bool)>(14, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 15 }, { QMetaType::Bool, 16 },
        }}),
        // Signal 'pointsCountChanged'
        QtMocHelpers::SignalData<void()>(17, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'addAddress'
        QtMocHelpers::MethodData<void(quint16, int)>(18, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UShort, 12 }, { QMetaType::Int, 19 },
        }}),
        // Method 'removeAddress'
        QtMocHelpers::MethodData<void(quint16)>(20, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UShort, 12 },
        }}),
        // Method 'getYMin'
        QtMocHelpers::MethodData<float(quint16) const>(21, 8, QMC::AccessPublic, QMetaType::Float, {{
            { QMetaType::UShort, 12 },
        }}),
        // Method 'getYMax'
        QtMocHelpers::MethodData<float(quint16) const>(22, 8, QMC::AccessPublic, QMetaType::Float, {{
            { QMetaType::UShort, 12 },
        }}),
        // Method 'setYMin'
        QtMocHelpers::MethodData<void(quint16, float)>(23, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UShort, 12 }, { QMetaType::Float, 24 },
        }}),
        // Method 'setYMax'
        QtMocHelpers::MethodData<void(quint16, float)>(25, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UShort, 12 }, { QMetaType::Float, 26 },
        }}),
        // Method 'play'
        QtMocHelpers::MethodData<void()>(27, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'pause'
        QtMocHelpers::MethodData<void()>(28, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'updateGraphSeries'
        QtMocHelpers::MethodData<void(quint16, QXYSeries *)>(29, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UShort, 12 }, { 0x80000000 | 30, 31 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'streamId'
        QtMocHelpers::PropertyData<quint8>(15, QMetaType::UChar, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'bufferSize'
        QtMocHelpers::PropertyData<int>(32, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'name'
        QtMocHelpers::PropertyData<QString>(33, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'frequency'
        QtMocHelpers::PropertyData<float>(34, QMetaType::Float, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'running'
        QtMocHelpers::PropertyData<bool>(35, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'addresses'
        QtMocHelpers::PropertyData<QList<quint16>>(36, 0x80000000 | 37, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 2),
        // property 'pointsCount'
        QtMocHelpers::PropertyData<int>(38, QMetaType::Int, QMC::DefaultPropertyFlags, 6),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
            {    5,    6 },
    });
    return QtMocHelpers::metaObjectData<Scope, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject Scope::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ScopeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ScopeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN5ScopeE_t>.metaTypes,
    nullptr
} };

void Scope::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Scope *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->nameChanged(); break;
        case 1: _t->runningChanged(); break;
        case 2: _t->addressesChanged(); break;
        case 3: _t->addressAdded((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1]))); break;
        case 4: _t->addressRemoved((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1]))); break;
        case 5: _t->streamControlRequested((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 6: _t->pointsCountChanged(); break;
        case 7: _t->addAddress((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->removeAddress((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1]))); break;
        case 9: { float _r = _t->getYMin((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1])));
            if (_a[0]) *reinterpret_cast<float*>(_a[0]) = std::move(_r); }  break;
        case 10: { float _r = _t->getYMax((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1])));
            if (_a[0]) *reinterpret_cast<float*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->setYMin((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[2]))); break;
        case 12: _t->setYMax((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[2]))); break;
        case 13: _t->play(); break;
        case 14: _t->pause(); break;
        case 15: _t->updateGraphSeries((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QXYSeries*>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::nameChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::runningChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::addressesChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)(quint16 )>(_a, &Scope::addressAdded, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)(quint16 )>(_a, &Scope::addressRemoved, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)(quint8 , bool )>(_a, &Scope::streamControlRequested, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::pointsCountChanged, 6))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint16> >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<quint8*>(_v) = _t->getStreamID(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->getBufferSize(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->getName(); break;
        case 3: *reinterpret_cast<float*>(_v) = _t->getFrequency(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->isRunning(); break;
        case 5: *reinterpret_cast<QList<quint16>*>(_v) = _t->getOrderedAddresses(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->pointsCount(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setName(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Scope::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Scope::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ScopeE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Scope::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Scope::nameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Scope::runningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Scope::addressesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Scope::addressAdded(quint16 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void Scope::addressRemoved(quint16 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void Scope::streamControlRequested(quint8 _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void Scope::pointsCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
