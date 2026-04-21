/****************************************************************************
** Meta object code from reading C++ file 'TcpWorker.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../sicstudio/TcpWorker.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TcpWorker.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9TcpWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto TcpWorker::qt_create_metaobjectdata<qt_meta_tag_ZN9TcpWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TcpWorker",
        "connectionStatusChanged",
        "",
        "connected",
        "dictionaryReceived",
        "QList<Protocol::DictionaryEntry>",
        "dictionary",
        "readValuesReceived",
        "QList<quint16>",
        "addresses",
        "QList<QVariant>",
        "values",
        "streamConfigured",
        "streamId",
        "streamStopped",
        "writeCompleted",
        "firstAddress",
        "count",
        "configCommitted",
        "infoMessage",
        "message",
        "errorOccurred",
        "errorMsg",
        "connectToAurix",
        "ip",
        "port",
        "disconnectFromAurix",
        "requestDictionary",
        "requestReadValues",
        "entries",
        "writeValues",
        "modifiedOnly",
        "configureStream",
        "frequencyX100",
        "stopStream",
        "commitConfig",
        "onReadyRead",
        "onConnected",
        "onDisconnected",
        "onSocketError",
        "QAbstractSocket::SocketError",
        "socketError",
        "onDictionaryFlushTimeout"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'connectionStatusChanged'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'dictionaryReceived'
        QtMocHelpers::SignalData<void(const QList<Protocol::DictionaryEntry> &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'readValuesReceived'
        QtMocHelpers::SignalData<void(const QVector<quint16> &, const QVector<QVariant> &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 }, { 0x80000000 | 10, 11 },
        }}),
        // Signal 'streamConfigured'
        QtMocHelpers::SignalData<void(quint8, const QVector<quint16> &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 13 }, { 0x80000000 | 8, 9 },
        }}),
        // Signal 'streamStopped'
        QtMocHelpers::SignalData<void(quint8)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 13 },
        }}),
        // Signal 'writeCompleted'
        QtMocHelpers::SignalData<void(quint16, quint8)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UShort, 16 }, { QMetaType::UChar, 17 },
        }}),
        // Signal 'configCommitted'
        QtMocHelpers::SignalData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'infoMessage'
        QtMocHelpers::SignalData<void(const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 20 },
        }}),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 },
        }}),
        // Slot 'connectToAurix'
        QtMocHelpers::SlotData<void(const QString &, quint16)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::UShort, 25 },
        }}),
        // Slot 'disconnectFromAurix'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'requestDictionary'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'requestReadValues'
        QtMocHelpers::SlotData<void(const QList<Protocol::DictionaryEntry> &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 29 },
        }}),
        // Slot 'writeValues'
        QtMocHelpers::SlotData<void(const QList<Protocol::DictionaryEntry> &, bool)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 29 }, { QMetaType::Bool, 31 },
        }}),
        // Slot 'writeValues'
        QtMocHelpers::SlotData<void(const QList<Protocol::DictionaryEntry> &)>(30, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 5, 29 },
        }}),
        // Slot 'configureStream'
        QtMocHelpers::SlotData<void(quint8, quint16, const QVector<quint16> &)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 13 }, { QMetaType::UShort, 33 }, { 0x80000000 | 8, 9 },
        }}),
        // Slot 'stopStream'
        QtMocHelpers::SlotData<void(quint8)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 13 },
        }}),
        // Slot 'commitConfig'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onReadyRead'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onConnected'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDisconnected'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSocketError'
        QtMocHelpers::SlotData<void(QAbstractSocket::SocketError)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 40, 41 },
        }}),
        // Slot 'onDictionaryFlushTimeout'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TcpWorker, qt_meta_tag_ZN9TcpWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TcpWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9TcpWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9TcpWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9TcpWorkerE_t>.metaTypes,
    nullptr
} };

void TcpWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TcpWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->connectionStatusChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->dictionaryReceived((*reinterpret_cast<std::add_pointer_t<QList<Protocol::DictionaryEntry>>>(_a[1]))); break;
        case 2: _t->readValuesReceived((*reinterpret_cast<std::add_pointer_t<QList<quint16>>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QList<QVariant>>>(_a[2]))); break;
        case 3: _t->streamConfigured((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QList<quint16>>>(_a[2]))); break;
        case 4: _t->streamStopped((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1]))); break;
        case 5: _t->writeCompleted((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint8>>(_a[2]))); break;
        case 6: _t->configCommitted(); break;
        case 7: _t->infoMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->errorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->connectToAurix((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint16>>(_a[2]))); break;
        case 10: _t->disconnectFromAurix(); break;
        case 11: _t->requestDictionary(); break;
        case 12: _t->requestReadValues((*reinterpret_cast<std::add_pointer_t<QList<Protocol::DictionaryEntry>>>(_a[1]))); break;
        case 13: _t->writeValues((*reinterpret_cast<std::add_pointer_t<QList<Protocol::DictionaryEntry>>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 14: _t->writeValues((*reinterpret_cast<std::add_pointer_t<QList<Protocol::DictionaryEntry>>>(_a[1]))); break;
        case 15: _t->configureStream((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint16>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QList<quint16>>>(_a[3]))); break;
        case 16: _t->stopStream((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1]))); break;
        case 17: _t->commitConfig(); break;
        case 18: _t->onReadyRead(); break;
        case 19: _t->onConnected(); break;
        case 20: _t->onDisconnected(); break;
        case 21: _t->onSocketError((*reinterpret_cast<std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 22: _t->onDictionaryFlushTimeout(); break;
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
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<Protocol::DictionaryEntry> >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<quint16> >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<quint16> >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<Protocol::DictionaryEntry> >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<Protocol::DictionaryEntry> >(); break;
            }
            break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<Protocol::DictionaryEntry> >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 2:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<quint16> >(); break;
            }
            break;
        case 21:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TcpWorker::*)(bool )>(_a, &TcpWorker::connectionStatusChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpWorker::*)(const QList<Protocol::DictionaryEntry> & )>(_a, &TcpWorker::dictionaryReceived, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpWorker::*)(const QVector<quint16> & , const QVector<QVariant> & )>(_a, &TcpWorker::readValuesReceived, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpWorker::*)(quint8 , const QVector<quint16> & )>(_a, &TcpWorker::streamConfigured, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpWorker::*)(quint8 )>(_a, &TcpWorker::streamStopped, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpWorker::*)(quint16 , quint8 )>(_a, &TcpWorker::writeCompleted, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpWorker::*)()>(_a, &TcpWorker::configCommitted, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpWorker::*)(const QString & )>(_a, &TcpWorker::infoMessage, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpWorker::*)(const QString & )>(_a, &TcpWorker::errorOccurred, 8))
            return;
    }
}

const QMetaObject *TcpWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9TcpWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TcpWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void TcpWorker::connectionStatusChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void TcpWorker::dictionaryReceived(const QList<Protocol::DictionaryEntry> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void TcpWorker::readValuesReceived(const QVector<quint16> & _t1, const QVector<QVariant> & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void TcpWorker::streamConfigured(quint8 _t1, const QVector<quint16> & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void TcpWorker::streamStopped(quint8 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void TcpWorker::writeCompleted(quint16 _t1, quint8 _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void TcpWorker::configCommitted()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TcpWorker::infoMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void TcpWorker::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}
QT_WARNING_POP
