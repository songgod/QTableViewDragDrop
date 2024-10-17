#include "tablemodel.h"
#include <QAbstractTableModel>
#include <QFlags>
#include <qabstractitemmodel.h>
#include <QMimeData>
#include <QIcon>

QVariant TableModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return dataList[index.row()][index.column()];
    }
    else if(index.column()==0)
    {
        if(role==Qt::CheckStateRole)
            return Qt::Checked;
        else if(role==Qt::DecorationRole)
            return QIcon(":/images/layer.png");
    }
    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    Q_UNUSED(role);
    if (role == Qt::EditRole)
    {
        dataList[index.row()][index.column()] = value;
    }
    emit dataChanged(index, index);
    return true;
}

int TableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return dataList.size();
}

int TableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 3;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const {
    Q_UNUSED(index);
    auto flags = Qt::ItemIsSelectable | Qt::ItemIsDropEnabled |  Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
    if(index.column()==0)
    {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

Qt::DropActions TableModel::supportedDropActions() const {
    auto flags = Qt::DropActions();
    flags |= Qt::MoveAction;
    return flags;
}

QStringList TableModel::mimeTypes() const {
    return { "application/vnd.text.list" };
}

QMimeData* TableModel::mimeData(const QModelIndexList& indexes) const {
    QMimeData* mimeData = QAbstractTableModel::mimeData(indexes);
    if (indexes.count() != 1)
        return mimeData;
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << indexes.at(0).row();
    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

bool TableModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column)
    if (!data->hasFormat("application/vnd.text.list"))
        return false;

    if (parent.isValid() || row == -1)
        return false;

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    int dragrow = -1;
    stream >> dragrow;

    if (dragrow == row)
        return false;

    if (dragrow < row)
        row -= 1;

    dataList.move(dragrow, row);

    emit selectedRowChanged(row);

    return true;
}
