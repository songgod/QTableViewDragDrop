#include <QApplication>
#include "tablemodel.h"
#include "tableview.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TableView* tableView = new TableView();

    TableModel* model = new TableModel();

    tableView->setModel(model);

    tableView->show();

    return a.exec();
}
