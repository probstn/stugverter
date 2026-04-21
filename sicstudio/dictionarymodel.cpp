#include "dictionarymodel.h"

#include <QtEndian>

DictionaryModel::DictionaryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int DictionaryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_entries.size();
}

QVariant DictionaryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size()) {
        return {};
    }

    const Entry &entry = m_entries[index.row()];
    switch (role) {
    case AddressRole:
        return static_cast<int>(entry.address);
    case NameRole:
        return entry.name;
    case UnitRole:
        return entry.unit;
    case TypeRole:
        return static_cast<int>(entry.type);
    case TypeNameRole:
        return typeName(entry.type);
    case AccessRole:
        return static_cast<int>(entry.access);
    case AccessNameRole:
        return accessName(entry.access);
    case ValueRole:
        return entry.value;
    case ValueTextRole:
        return valueToText(entry.value, entry.type);
    case EditableRole:
        return entry.access == AccessWriteAfterRestart || entry.access == AccessWriteLive;
    case PlotEnabledRole:
        return entry.plotEnabled;
    default:
        return {};
    }
}

QHash<int, QByteArray> DictionaryModel::roleNames() const
{
    return {
        {AddressRole, "address"},
        {NameRole, "name"},
        {UnitRole, "unit"},
        {TypeRole, "type"},
        {TypeNameRole, "typeName"},
        {AccessRole, "access"},
        {AccessNameRole, "accessName"},
        {ValueRole, "value"},
        {ValueTextRole, "valueText"},
        {EditableRole, "editable"},
        {PlotEnabledRole, "plotEnabled"}
    };
}

void DictionaryModel::clear()
{
    beginResetModel();
    m_entries.clear();
    m_rowByAddress.clear();
    endResetModel();
}

void DictionaryModel::setEntries(const QVector<Entry> &entries)
{
    beginResetModel();
    m_entries = entries;
    rebuildIndex();
    endResetModel();
}

bool DictionaryModel::hasAddress(quint16 address) const
{
    return m_rowByAddress.contains(address);
}


bool DictionaryModel::metadataForAddress(quint16 address, quint8 *type, quint8 *access, QString *name, QString *unit) const
{
    const int row = m_rowByAddress.value(address, -1);
    if (row < 0) {
        return false;
    }

    const Entry &entry = m_entries[row];
    if (type != nullptr) {
        *type = entry.type;
    }
    if (access != nullptr) {
        *access = entry.access;
    }
    if (name != nullptr) {
        *name = entry.name;
    }
    if (unit != nullptr) {
        *unit = entry.unit;
    }
    return true;
}

int DictionaryModel::byteSizeForAddress(quint16 address) const
{
    quint8 type = 0;
    if (!metadataForAddress(address, &type, nullptr, nullptr, nullptr)) {
        return 0;
    }
    return byteSizeForType(type);
}

QString DictionaryModel::nameForAddress(quint16 address) const
{
    QString name;
    metadataForAddress(address, nullptr, nullptr, &name, nullptr);
    return name;
}

bool DictionaryModel::updateValue(quint16 address, const QVariant &value)
{
    const int row = m_rowByAddress.value(address, -1);
    if (row < 0) {
        return false;
    }

    Entry &entry = m_entries[row];
    entry.value = value;

    const QModelIndex changed = index(row, 0);
    emit dataChanged(changed, changed, {ValueRole, ValueTextRole});
    return true;
}

bool DictionaryModel::setPlotEnabledByAddress(quint16 address, bool enabled)
{
    const int row = m_rowByAddress.value(address, -1);
    if (row < 0) {
        return false;
    }

    Entry &entry = m_entries[row];
    if (entry.plotEnabled == enabled) {
        return true;
    }

    entry.plotEnabled = enabled;
    const QModelIndex changed = index(row, 0);
    emit dataChanged(changed, changed, {PlotEnabledRole});
    return true;
}

QVariantList DictionaryModel::selectedPlotAddresses() const
{
    QVariantList out;
    out.reserve(m_entries.size());

    for (const Entry &entry : m_entries) {
        if (entry.plotEnabled) {
            out.push_back(static_cast<int>(entry.address));
        }
    }

    return out;
}

void DictionaryModel::setPlotEnabledByRow(int row, bool enabled)
{
    if (row < 0 || row >= m_entries.size()) {
        return;
    }
    setPlotEnabledByAddress(m_entries[row].address, enabled);
}

int DictionaryModel::addressAt(int row) const
{
    if (row < 0 || row >= m_entries.size()) {
        return 0;
    }
    return static_cast<int>(m_entries[row].address);
}

bool DictionaryModel::isEditableAt(int row) const
{
    if (row < 0 || row >= m_entries.size()) {
        return false;
    }
    const quint8 access = m_entries[row].access;
    return access == AccessWriteAfterRestart || access == AccessWriteLive;
}

int DictionaryModel::byteSizeForType(quint8 type)
{
    switch (type) {
    case TypeUInt8:
    case TypeInt8:
        return 1;
    case TypeUInt16:
    case TypeInt16:
        return 2;
    case TypeUInt32:
    case TypeInt32:
    case TypeFloat32:
        return 4;
    case TypeUInt64:
        return 8;
    default:
        return 0;
    }
}

QString DictionaryModel::typeName(quint8 type)
{
    switch (type) {
    case TypeUInt8:
        return QStringLiteral("u8");
    case TypeInt8:
        return QStringLiteral("i8");
    case TypeUInt16:
        return QStringLiteral("u16");
    case TypeInt16:
        return QStringLiteral("i16");
    case TypeUInt32:
        return QStringLiteral("u32");
    case TypeInt32:
        return QStringLiteral("i32");
    case TypeFloat32:
        return QStringLiteral("f32");
    case TypeUInt64:
        return QStringLiteral("u64");
    default:
        return QStringLiteral("?");
    }
}

QString DictionaryModel::accessName(quint8 access)
{
    switch (access) {
    case AccessReadOnly:
        return QStringLiteral("RO");
    case AccessWriteAfterRestart:
        return QStringLiteral("WR");
    case AccessWriteLive:
        return QStringLiteral("WL");
    default:
        return QStringLiteral("?");
    }
}

QString DictionaryModel::valueToText(const QVariant &value, quint8 type)
{
    if (!value.isValid()) {
        return QStringLiteral("-");
    }

    switch (type) {
    case TypeFloat32:
        return QString::number(value.toDouble(), 'f', 4);
    case TypeUInt64:
        return QString::number(value.toULongLong());
    default:
        return value.toString();
    }
}

void DictionaryModel::rebuildIndex()
{
    m_rowByAddress.clear();
    for (int i = 0; i < m_entries.size(); ++i) {
        m_rowByAddress.insert(m_entries[i].address, i);
    }
}