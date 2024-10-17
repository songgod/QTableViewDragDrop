#pragma once

#include <QTableView>


class TableView: public QTableView {

    Q_OBJECT

public:

    TableView(QWidget* parent=nullptr);

public:

    virtual void setModel(QAbstractItemModel* model) override;
};
