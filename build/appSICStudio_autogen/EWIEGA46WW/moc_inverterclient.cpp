/****************************************************************************
** Meta object code from reading C++ file 'inverterclient.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../sicstudio/inverterclient.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inverterclient.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14InverterClientE_t {};
} // unnamed namespace

template <> constexpr inline auto InverterClient::qt_create_metaobjectdata<qt_meta_tag_ZN14InverterClientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "InverterClient",
        "connectedChanged",
        "",
        "statusChanged",
        "dictionaryReloaded",
        "streamSample",
        "address",
        "name",
        "tSec",
        "value",
        "streamSamplesReady",
        "QList<PlotSample>",
        "samples",
        "commandError",
        "text",
        "onTcpConnected",
        "onTcpDisconnected",
        "onTcpError",
        "QAbstractSocket::SocketError",
        "socketError",
        "onTcpReadyRead",
        "onUdpReadyRead",
        "finalizeDictionary",
        "connectToDevice",
        "host",
        "port",
        "disconnectFromDevice",
        "requestDictionary",
        "readAddresses",
        "QVariantList",
        "addresses",
        "readAllDictionaryValues",
        "writeValue",
        "valueText",
        "startStream",
        "streamId",
        "loopDivider",
        "stopStream",
        "commitConfig",
        "connected",
        "status"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'connectedChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'statusChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dictionaryReloaded'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'streamSample'
        QtMocHelpers::SignalData<void(int, QString, double, double)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::QString, 7 }, { QMetaType::Double, 8 }, { QMetaType::Double, 9 },
        }}),
        // Signal 'streamSamplesReady'
        QtMocHelpers::SignalData<void(const QVector<PlotSample> &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Signal 'commandError'
        QtMocHelpers::SignalData<void(QString)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
        // Slot 'onTcpConnected'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTcpDisconnected'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTcpError'
        QtMocHelpers::SlotData<void(QAbstractSocket::SocketError)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Slot 'onTcpReadyRead'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onUdpReadyRead'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'finalizeDictionary'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'connectToDevice'
        QtMocHelpers::MethodData<void(const QString &, int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::Int, 25 },
        }}),
        // Method 'connectToDevice'
        QtMocHelpers::MethodData<void(const QString &)>(23, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 24 },
        }}),
        // Method 'disconnectFromDevice'
        QtMocHelpers::MethodData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'requestDictionary'
        QtMocHelpers::MethodData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'readAddresses'
        QtMocHelpers::MethodData<void(const QVariantList &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 29, 30 },
        }}),
        // Method 'readAllDictionaryValues'
        QtMocHelpers::MethodData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'writeValue'
        QtMocHelpers::MethodData<void(int, const QString &)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::QString, 33 },
        }}),
        // Method 'startStream'
        QtMocHelpers::MethodData<void(int, int, const QVariantList &)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 35 }, { QMetaType::Int, 36 }, { 0x80000000 | 29, 30 },
        }}),
        // Method 'stopStream'
        QtMocHelpers::MethodData<void(int)>(37, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 35 },
        }}),
        // Method 'commitConfig'
        QtMocHelpers::MethodData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'connected'
        QtMocHelpers::PropertyData<bool>(39, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'status'
        QtMocHelpers::PropertyData<QString>(40, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<InverterClient, qt_meta_tag_ZN14InverterClientE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject InverterClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14InverterClientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14InverterClientE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14InverterClientE_t>.metaTypes,
    nullptr
} };

void InverterClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<InverterClient *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->connectedChanged(); break;
        case 1: _t->statusChanged(); break;
        case 2: _t->dictionaryReloaded(); break;
        case 3: _t->streamSample((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4]))); break;
        case 4: _t->streamSamplesReady((*reinterpret_cast<std::add_pointer_t<QList<PlotSample>>>(_a[1]))); break;
        case 5: _t->commandError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->onTcpConnected(); break;
        case 7: _t->onTcpDisconnected(); break;
        case 8: _t->onTcpError((*reinterpret_cast<std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 9: _t->onTcpReadyRead(); break;
        case 10: _t->onUdpReadyRead(); break;
        case 11: _t->finalizeDictionary(); break;
        case 12: _t->connectToDevice((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 13: _t->connectToDevice((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->disconnectFromDevice(); break;
        case 15: _t->requestDictionary(); break;
        case 16: _t->readAddresses((*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[1]))); break;
        case 17: _t->readAllDictionaryValues(); break;
        case 18: _t->writeValue((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 19: _t->startStream((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[3]))); break;
        case 20: _t->stopStream((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->commitConfig(); break;
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
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (InverterClient::*)()>(_a, &InverterClient::connectedChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterClient::*)()>(_a, &InverterClient::statusChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterClient::*)()>(_a, &InverterClient::dictionaryReloaded, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterClient::*)(int , QString , double , double )>(_a, &InverterClient::streamSample, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterClient::*)(const QVector<PlotSample> & )>(_a, &InverterClient::streamSamplesReady, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (InverterClient::*)(QString )>(_a, &InverterClient::commandError, 5))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->connected(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->status(); break;
        default: break;
        }
    }
}

const QMetaObject *InverterClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InverterClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14InverterClientE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int InverterClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
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
void InverterClient::connectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void InverterClient::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void InverterClient::dictionaryReloaded()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void InverterClient::streamSample(int _t1, QString _t2, double _t3, double _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 4
void InverterClient::streamSamplesReady(const QVector<PlotSample> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void InverterClient::commandError(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
