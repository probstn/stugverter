/****************************************************************************
** Meta object code from reading C++ file 'InverterProtocol.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../stuglab/InverterProtocol.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InverterProtocol.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16InverterProtocolE_t {};
} // unnamed namespace

template <> constexpr inline auto InverterProtocol::qt_create_metaobjectdata<qt_meta_tag_ZN16InverterProtocolE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "InverterProtocol",
        "dictionaryAckReceived",
        "",
        "payload",
        "readAckReceived",
        "writeAckReceived",
        "cnt",
        "nackReceived",
        "errorCode",
        "errorOccurred",
        "errorMsg",
        "streamDataReceived",
        "data",
        "streamStopAckReceived",
        "streamId",
        "onDataReceived"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'dictionaryAckReceived'
        QtMocHelpers::SignalData<void(const QByteArray &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QByteArray, 3 },
        }}),
        // Signal 'readAckReceived'
        QtMocHelpers::SignalData<void(const QByteArray &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QByteArray, 3 },
        }}),
        // Signal 'writeAckReceived'
        QtMocHelpers::SignalData<void(quint8)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 6 },
        }}),
        // Signal 'nackReceived'
        QtMocHelpers::SignalData<void(quint8)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 8 },
        }}),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Signal 'streamDataReceived'
        QtMocHelpers::SignalData<void(const QByteArray &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QByteArray, 12 },
        }}),
        // Signal 'streamStopAckReceived'
        QtMocHelpers::SignalData<void(quint8)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 14 },
        }}),
        // Slot 'onDataReceived'
        QtMocHelpers::SlotData<void(const QByteArray &)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QByteArray, 12 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<InverterProtocol, qt_meta_tag_ZN16InverterProtocolE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject InverterProtocol::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16InverterProtocolE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16InverterProtocolE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16InverterProtocolE_t>.metaTypes,
    nullptr
} };

void InverterProtocol::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<InverterProtocol *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->dictionaryAckReceived((*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 1: _t->readAckReceived((*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 2: _t->writeAckReceived((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1]))); break;
        case 3: _t->nackReceived((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1]))); break;
        case 4: _t->errorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->streamDataReceived((*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 6: _t->streamStopAckReceived((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1]))); break;
        case 7: _t->onDataReceived((*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (InverterProtocol::*)(const QByteArray & )>(_a, &InverterProtocol::dictionaryAckReceived, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterProtocol::*)(const QByteArray & )>(_a, &InverterProtocol::readAckReceived, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterProtocol::*)(quint8 )>(_a, &InverterProtocol::writeAckReceived, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterProtocol::*)(quint8 )>(_a, &InverterProtocol::nackReceived, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterProtocol::*)(const QString & )>(_a, &InverterProtocol::errorOccurred, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterProtocol::*)(const QByteArray & )>(_a, &InverterProtocol::streamDataReceived, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterProtocol::*)(quint8 )>(_a, &InverterProtocol::streamStopAckReceived, 6))
            return;
    }
}

const QMetaObject *InverterProtocol::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InverterProtocol::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16InverterProtocolE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int InverterProtocol::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void InverterProtocol::dictionaryAckReceived(const QByteArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void InverterProtocol::readAckReceived(const QByteArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void InverterProtocol::writeAckReceived(quint8 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void InverterProtocol::nackReceived(quint8 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void InverterProtocol::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void InverterProtocol::streamDataReceived(const QByteArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void InverterProtocol::streamStopAckReceived(quint8 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}
QT_WARNING_POP
