/****************************************************************************
** Meta object code from reading C++ file 'ScopeManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../stuglab/ScopeManager.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScopeManager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12ScopeManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto ScopeManager::qt_create_metaobjectdata<qt_meta_tag_ZN12ScopeManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ScopeManager",
        "QML.Element",
        "auto",
        "QML.Singleton",
        "true",
        "scopesChanged",
        "",
        "scopeAdded",
        "Scope*",
        "scope",
        "scopeRemoved",
        "id",
        "processStreamPayload",
        "payload",
        "removeScope",
        "addScope",
        "frequency",
        "bufferSize",
        "scopes",
        "QList<Scope*>"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'scopesChanged'
        QtMocHelpers::SignalData<void()>(5, 6, QMC::AccessPublic, QMetaType::Void),
        // Signal 'scopeAdded'
        QtMocHelpers::SignalData<void(Scope *)>(7, 6, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Signal 'scopeRemoved'
        QtMocHelpers::SignalData<void(quint8)>(10, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 11 },
        }}),
        // Slot 'processStreamPayload'
        QtMocHelpers::SlotData<void(const QByteArray &)>(12, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QByteArray, 13 },
        }}),
        // Method 'removeScope'
        QtMocHelpers::MethodData<void(quint8)>(14, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 11 },
        }}),
        // Method 'addScope'
        QtMocHelpers::MethodData<void(quint8, int)>(15, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 16 }, { QMetaType::Int, 17 },
        }}),
        // Method 'addScope'
        QtMocHelpers::MethodData<void(quint8)>(15, 6, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::UChar, 16 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'scopes'
        QtMocHelpers::PropertyData<QList<Scope*>>(18, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
    });
    return QtMocHelpers::metaObjectData<ScopeManager, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject ScopeManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ScopeManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ScopeManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12ScopeManagerE_t>.metaTypes,
    nullptr
} };

void ScopeManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScopeManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->scopesChanged(); break;
        case 1: _t->scopeAdded((*reinterpret_cast<std::add_pointer_t<Scope*>>(_a[1]))); break;
        case 2: _t->scopeRemoved((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1]))); break;
        case 3: _t->processStreamPayload((*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 4: _t->removeScope((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1]))); break;
        case 5: _t->addScope((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->addScope((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Scope* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ScopeManager::*)()>(_a, &ScopeManager::scopesChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScopeManager::*)(Scope * )>(_a, &ScopeManager::scopeAdded, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScopeManager::*)(quint8 )>(_a, &ScopeManager::scopeRemoved, 2))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<Scope*> >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QList<Scope*>*>(_v) = _t->getScopes(); break;
        default: break;
        }
    }
}

const QMetaObject *ScopeManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScopeManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ScopeManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ScopeManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ScopeManager::scopesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ScopeManager::scopeAdded(Scope * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void ScopeManager::scopeRemoved(quint8 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
