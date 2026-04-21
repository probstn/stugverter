/****************************************************************************
** Meta object code from reading C++ file 'Scope.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../sicstudio/Scope.h"
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
        "connectedChanged",
        "",
        "streamingChanged",
        "statusTextChanged",
        "tcpPortChanged",
        "udpPortChanged",
        "streamFrequencyHzChanged",
        "scopeDataChanged",
        "onTcpConnectedChanged",
        "connected",
        "onDictionaryReceived",
        "QList<Protocol::DictionaryEntry>",
        "dictionary",
        "onReadValuesReceived",
        "QList<quint16>",
        "addresses",
        "QList<QVariant>",
        "values",
        "onStreamConfigured",
        "streamId",
        "onStreamStopped",
        "onWriteCompleted",
        "firstAddress",
        "count",
        "onConfigCommitted",
        "onWorkerInfo",
        "message",
        "onWorkerError",
        "onUdpValuesReceived",
        "sequence",
        "timestamp",
        "onRenderTick",
        "connectSystem",
        "ip",
        "disconnectSystem",
        "requestDictionary",
        "readConfig",
        "writeConfig",
        "commitConfig",
        "startStreaming",
        "stopStreaming",
        "scopeSeries",
        "QVariantList",
        "scopeIndex",
        "selectedStreamCount",
        "dictionaryModel",
        "streaming",
        "statusText",
        "tcpPort",
        "udpPort",
        "streamFrequencyHz"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'connectedChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'streamingChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'statusTextChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'tcpPortChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'udpPortChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'streamFrequencyHzChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'scopeDataChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onTcpConnectedChanged'
        QtMocHelpers::SlotData<void(bool)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 10 },
        }}),
        // Slot 'onDictionaryReceived'
        QtMocHelpers::SlotData<void(const QList<Protocol::DictionaryEntry> &)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Slot 'onReadValuesReceived'
        QtMocHelpers::SlotData<void(const QVector<quint16> &, const QVector<QVariant> &)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 15, 16 }, { 0x80000000 | 17, 18 },
        }}),
        // Slot 'onStreamConfigured'
        QtMocHelpers::SlotData<void(quint8, const QVector<quint16> &)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UChar, 20 }, { 0x80000000 | 15, 16 },
        }}),
        // Slot 'onStreamStopped'
        QtMocHelpers::SlotData<void(quint8)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UChar, 20 },
        }}),
        // Slot 'onWriteCompleted'
        QtMocHelpers::SlotData<void(quint16, quint8)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UShort, 23 }, { QMetaType::UChar, 24 },
        }}),
        // Slot 'onConfigCommitted'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onWorkerInfo'
        QtMocHelpers::SlotData<void(const QString &)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 27 },
        }}),
        // Slot 'onWorkerError'
        QtMocHelpers::SlotData<void(const QString &)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 27 },
        }}),
        // Slot 'onUdpValuesReceived'
        QtMocHelpers::SlotData<void(quint8, quint16, quint64, const QVector<QVariant> &)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UChar, 20 }, { QMetaType::UShort, 30 }, { QMetaType::ULongLong, 31 }, { 0x80000000 | 17, 18 },
        }}),
        // Slot 'onRenderTick'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'connectSystem'
        QtMocHelpers::MethodData<void(const QString &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
        // Method 'disconnectSystem'
        QtMocHelpers::MethodData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'requestDictionary'
        QtMocHelpers::MethodData<void()>(36, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'readConfig'
        QtMocHelpers::MethodData<void()>(37, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'writeConfig'
        QtMocHelpers::MethodData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'commitConfig'
        QtMocHelpers::MethodData<void()>(39, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'startStreaming'
        QtMocHelpers::MethodData<void()>(40, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stopStreaming'
        QtMocHelpers::MethodData<void()>(41, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'scopeSeries'
        QtMocHelpers::MethodData<QVariantList(int) const>(42, 2, QMC::AccessPublic, 0x80000000 | 43, {{
            { QMetaType::Int, 44 },
        }}),
        // Method 'selectedStreamCount'
        QtMocHelpers::MethodData<int() const>(45, 2, QMC::AccessPublic, QMetaType::Int),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'dictionaryModel'
        QtMocHelpers::PropertyData<QObject*>(46, QMetaType::QObjectStar, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'connected'
        QtMocHelpers::PropertyData<bool>(10, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'streaming'
        QtMocHelpers::PropertyData<bool>(47, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'statusText'
        QtMocHelpers::PropertyData<QString>(48, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
        // property 'tcpPort'
        QtMocHelpers::PropertyData<int>(49, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'udpPort'
        QtMocHelpers::PropertyData<int>(50, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'streamFrequencyHz'
        QtMocHelpers::PropertyData<int>(51, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Scope, qt_meta_tag_ZN5ScopeE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
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
        case 0: _t->connectedChanged(); break;
        case 1: _t->streamingChanged(); break;
        case 2: _t->statusTextChanged(); break;
        case 3: _t->tcpPortChanged(); break;
        case 4: _t->udpPortChanged(); break;
        case 5: _t->streamFrequencyHzChanged(); break;
        case 6: _t->scopeDataChanged(); break;
        case 7: _t->onTcpConnectedChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->onDictionaryReceived((*reinterpret_cast<std::add_pointer_t<QList<Protocol::DictionaryEntry>>>(_a[1]))); break;
        case 9: _t->onReadValuesReceived((*reinterpret_cast<std::add_pointer_t<QList<quint16>>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QList<QVariant>>>(_a[2]))); break;
        case 10: _t->onStreamConfigured((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QList<quint16>>>(_a[2]))); break;
        case 11: _t->onStreamStopped((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1]))); break;
        case 12: _t->onWriteCompleted((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint8>>(_a[2]))); break;
        case 13: _t->onConfigCommitted(); break;
        case 14: _t->onWorkerInfo((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->onWorkerError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->onUdpValuesReceived((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint16>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<quint64>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QList<QVariant>>>(_a[4]))); break;
        case 17: _t->onRenderTick(); break;
        case 18: _t->connectSystem((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: _t->disconnectSystem(); break;
        case 20: _t->requestDictionary(); break;
        case 21: _t->readConfig(); break;
        case 22: _t->writeConfig(); break;
        case 23: _t->commitConfig(); break;
        case 24: _t->startStreaming(); break;
        case 25: _t->stopStreaming(); break;
        case 26: { QVariantList _r = _t->scopeSeries((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 27: { int _r = _t->selectedStreamCount();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<Protocol::DictionaryEntry> >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<quint16> >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<quint16> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::connectedChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::streamingChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::statusTextChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::tcpPortChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::udpPortChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::streamFrequencyHzChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scope::*)()>(_a, &Scope::scopeDataChanged, 6))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QObject**>(_v) = _t->dictionaryModel(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->connected(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->streaming(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->statusText(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->tcpPort(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->udpPort(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->streamFrequencyHz(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 4: _t->setTcpPort(*reinterpret_cast<int*>(_v)); break;
        case 5: _t->setUdpPort(*reinterpret_cast<int*>(_v)); break;
        case 6: _t->setStreamFrequencyHz(*reinterpret_cast<int*>(_v)); break;
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
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
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
void Scope::connectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Scope::streamingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Scope::statusTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Scope::tcpPortChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Scope::udpPortChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Scope::streamFrequencyHzChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Scope::scopeDataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
