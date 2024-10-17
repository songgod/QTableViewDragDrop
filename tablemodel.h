#pragma once

#include <QAbstractTableModel>

using std::vector;

class TableModel : public QAbstractTableModel {

    Q_OBJECT

public:

    QVector<QVector<QVariant>> dataList{
        {"1", "a", "A"},
        {"2", "b", "B"},
        {"3", "c", "C"},
        {"4", "d", "D"},
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Qt::DropActions supportedDropActions() const override;

    QStringList mimeTypes() const override;

    QMimeData* mimeData(const QModelIndexList& indexes) const override;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &index) override;

signals:

    void selectedRowChanged(int row);

};


