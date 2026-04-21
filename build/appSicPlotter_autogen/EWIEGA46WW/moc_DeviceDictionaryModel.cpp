/****************************************************************************
** Meta object code from reading C++ file 'DeviceDictionaryModel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../sicstudio/DeviceDictionaryModel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DeviceDictionaryModel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN21DeviceDictionaryModelE_t {};
} // unnamed namespace

template <> constexpr inline auto DeviceDictionaryModel::qt_create_metaobjectdata<qt_meta_tag_ZN21DeviceDictionaryModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DeviceDictionaryModel",
        "countChanged",
        "",
        "clearStreamSelection",
        "setPendingValue",
        "row",
        "text",
        "setStreamSelected",
        "selected",
        "isWritable",
        "count",
        "Roles",
        "AddressRole",
        "AddressHexRole",
        "NameRole",
        "UnitRole",
        "TypeCodeRole",
        "TypeNameRole",
        "AccessCodeRole",
        "AccessNameRole",
        "ValueRole",
        "PendingValueRole",
        "WritableRole",
        "ModifiedRole",
        "StreamSelectedRole"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'clearStreamSelection'
        QtMocHelpers::MethodData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'setPendingValue'
        QtMocHelpers::MethodData<bool(int, const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 5 }, { QMetaType::QString, 6 },
        }}),
        // Method 'setStreamSelected'
        QtMocHelpers::MethodData<void(int, bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 }, { QMetaType::Bool, 8 },
        }}),
        // Method 'isWritable'
        QtMocHelpers::MethodData<bool(int) const>(9, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 5 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'count'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'Roles'
        QtMocHelpers::EnumData<enum Roles>(11, 11, QMC::EnumFlags{}).add({
            {   12, Roles::AddressRole },
            {   13, Roles::AddressHexRole },
            {   14, Roles::NameRole },
            {   15, Roles::UnitRole },
            {   16, Roles::TypeCodeRole },
            {   17, Roles::TypeNameRole },
            {   18, Roles::AccessCodeRole },
            {   19, Roles::AccessNameRole },
            {   20, Roles::ValueRole },
            {   21, Roles::PendingValueRole },
            {   22, Roles::WritableRole },
            {   23, Roles::ModifiedRole },
            {   24, Roles::StreamSelectedRole },
        }),
    };
    return QtMocHelpers::metaObjectData<DeviceDictionaryModel, qt_meta_tag_ZN21DeviceDictionaryModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DeviceDictionaryModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21DeviceDictionaryModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21DeviceDictionaryModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN21DeviceDictionaryModelE_t>.metaTypes,
    nullptr
} };

void DeviceDictionaryModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DeviceDictionaryModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->countChanged(); break;
        case 1: _t->clearStreamSelection(); break;
        case 2: { bool _r = _t->setPendingValue((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->setStreamSelected((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 4: { bool _r = _t->isWritable((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DeviceDictionaryModel::*)()>(_a, &DeviceDictionaryModel::countChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->count(); break;
        default: break;
        }
    }
}

const QMetaObject *DeviceDictionaryModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeviceDictionaryModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21DeviceDictionaryModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int DeviceDictionaryModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
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
void DeviceDictionaryModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
