/****************************************************************************
** Meta object code from reading C++ file 'UdpWorker.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../sicstudio/UdpWorker.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UdpWorker.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9UdpWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto UdpWorker::qt_create_metaobjectdata<qt_meta_tag_ZN9UdpWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "UdpWorker",
        "valuesReceived",
        "",
        "streamId",
        "sequence",
        "timestamp",
        "QList<QVariant>",
        "values",
        "errorOccurred",
        "errorMessage",
        "startListening",
        "port",
        "stopListening",
        "setStreamLayout",
        "QList<Protocol::DictionaryEntry>",
        "entries",
        "processPendingDatagrams"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'valuesReceived'
        QtMocHelpers::SignalData<void(quint8, quint16, quint64, const QVector<QVariant> &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 3 }, { QMetaType::UShort, 4 }, { QMetaType::ULongLong, 5 }, { 0x80000000 | 6, 7 },
        }}),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Slot 'startListening'
        QtMocHelpers::SlotData<void(quint16)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UShort, 11 },
        }}),
        // Slot 'stopListening'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setStreamLayout'
        QtMocHelpers::SlotData<void(quint8, const QList<Protocol::DictionaryEntry> &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 3 }, { 0x80000000 | 14, 15 },
        }}),
        // Slot 'processPendingDatagrams'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<UdpWorker, qt_meta_tag_ZN9UdpWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject UdpWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UdpWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UdpWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9UdpWorkerE_t>.metaTypes,
    nullptr
} };

void UdpWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<UdpWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->valuesReceived((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint16>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<quint64>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QList<QVariant>>>(_a[4]))); break;
        case 1: _t->errorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->startListening((*reinterpret_cast<std::add_pointer_t<quint16>>(_a[1]))); break;
        case 3: _t->stopListening(); break;
        case 4: _t->setStreamLayout((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QList<Protocol::DictionaryEntry>>>(_a[2]))); break;
        case 5: _t->processPendingDatagrams(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<Protocol::DictionaryEntry> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (UdpWorker::*)(quint8 , quint16 , quint64 , const QVector<QVariant> & )>(_a, &UdpWorker::valuesReceived, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (UdpWorker::*)(const QString & )>(_a, &UdpWorker::errorOccurred, 1))
            return;
    }
}

const QMetaObject *UdpWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UdpWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UdpWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UdpWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void UdpWorker::valuesReceived(quint8 _t1, quint16 _t2, quint64 _t3, const QVector<QVariant> & _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 1
void UdpWorker::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
