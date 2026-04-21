/****************************************************************************
** Meta object code from reading C++ file 'InverterManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../stuglab/InverterManager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InverterManager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN15InverterManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto InverterManager::qt_create_metaobjectdata<qt_meta_tag_ZN15InverterManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "InverterManager",
        "errorOccurred",
        "",
        "message",
        "writeCompleted",
        "handleConnectionStatusChange",
        "onScopeAdded",
        "Scope*",
        "scope",
        "onScopeRemoved",
        "id",
        "onStreamControlRequested",
        "streamId",
        "start",
        "getEntryValue",
        "QVariant",
        "readConfiguration",
        "writeConfiguration",
        "setUserInputValue",
        "row",
        "value",
        "dictionary",
        "DictionaryManager*",
        "scopeManager",
        "ScopeManager*"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'writeCompleted'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'handleConnectionStatusChange'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onScopeAdded'
        QtMocHelpers::SlotData<void(Scope *)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'onScopeRemoved'
        QtMocHelpers::SlotData<void(quint8)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UChar, 10 },
        }}),
        // Slot 'onStreamControlRequested'
        QtMocHelpers::SlotData<void(quint8, bool)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UChar, 12 }, { QMetaType::Bool, 13 },
        }}),
        // Method 'getEntryValue'
        QtMocHelpers::MethodData<QVariant(quint16) const>(14, 2, QMC::AccessPublic, 0x80000000 | 15, {{
            { QMetaType::UShort, 10 },
        }}),
        // Method 'readConfiguration'
        QtMocHelpers::MethodData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'writeConfiguration'
        QtMocHelpers::MethodData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'setUserInputValue'
        QtMocHelpers::MethodData<void(int, const QVariant &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 19 }, { 0x80000000 | 15, 20 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'dictionary'
        QtMocHelpers::PropertyData<DictionaryManager*>(21, 0x80000000 | 22, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'scopeManager'
        QtMocHelpers::PropertyData<ScopeManager*>(23, 0x80000000 | 24, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<InverterManager, qt_meta_tag_ZN15InverterManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject InverterManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15InverterManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15InverterManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15InverterManagerE_t>.metaTypes,
    nullptr
} };

void InverterManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<InverterManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->errorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->writeCompleted(); break;
        case 2: _t->handleConnectionStatusChange(); break;
        case 3: _t->onScopeAdded((*reinterpret_cast<std::add_pointer_t<Scope*>>(_a[1]))); break;
        case 4: _t->onScopeRemoved((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1]))); break;
        case 5: _t->onStreamControlRequested((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 6: { QVariant _r = _t->getEntryValue((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariant*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->readConfiguration(); break;
        case 8: _t->writeConfiguration(); break;
        case 9: _t->setUserInputValue((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QVariant>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Scope* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (InverterManager::*)(const QString & )>(_a, &InverterManager::errorOccurred, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterManager::*)()>(_a, &InverterManager::writeCompleted, 1))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DictionaryManager* >(); break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ScopeManager* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<DictionaryManager**>(_v) = _t->dictionary(); break;
        case 1: *reinterpret_cast<ScopeManager**>(_v) = _t->scopeManager(); break;
        default: break;
        }
    }
}

const QMetaObject *InverterManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InverterManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15InverterManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int InverterManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void InverterManager::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void InverterManager::writeCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
