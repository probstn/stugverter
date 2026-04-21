#ifndef DICTIONARYMODEL_H
#define DICTIONARYMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QVariant>
#include <QVector>

class DictionaryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DataType : quint8 {
        TypeUInt8 = 0,
        TypeInt8 = 1,
        TypeUInt16 = 2,
        TypeInt16 = 3,
        TypeUInt32 = 4,
        TypeInt32 = 5,
        TypeFloat32 = 6,
        TypeUInt64 = 7
    };

    enum AccessType : quint8 {
        AccessReadOnly = 0,
        AccessWriteAfterRestart = 1,
        AccessWriteLive = 2
    };

    enum Roles {
        AddressRole = Qt::UserRole + 1,
        NameRole,
        UnitRole,
        TypeRole,
        TypeNameRole,
        AccessRole,
        AccessNameRole,
        ValueRole,
        ValueTextRole,
        EditableRole,
        PlotEnabledRole
    };

    struct Entry {
        quint16 address = 0;
        quint8 type = 0;
        quint8 access = 0;
        QString name;
        QString unit;
        QVariant value;
        bool plotEnabled = false;
    };

    explicit DictionaryModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void clear();
    void setEntries(const QVector<Entry> &entries);

    bool hasAddress(quint16 address) const;
    bool metadataForAddress(quint16 address, quint8 *type, quint8 *access, QString *name, QString *unit) const;
    int byteSizeForAddress(quint16 address) const;
    QString nameForAddress(quint16 address) const;

    bool updateValue(quint16 address, const QVariant &value);
    bool setPlotEnabledByAddress(quint16 address, bool enabled);

    Q_INVOKABLE QVariantList selectedPlotAddresses() const;
    Q_INVOKABLE void setPlotEnabledByRow(int row, bool enabled);
    Q_INVOKABLE int addressAt(int row) const;
    Q_INVOKABLE bool isEditableAt(int row) const;

    static int byteSizeForType(quint8 type);
    static QString typeName(quint8 type);
    static QString accessName(quint8 access);
    static QString valueToText(const QVariant &value, quint8 type);

private:
    QVector<Entry> m_entries;
    QHash<quint16, int> m_rowByAddress;

    void rebuildIndex();
};

#endif