#include "tableview.h"
#include "tablemodel.h"
#include <QDropEvent>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QPainter>
#include <QProxyStyle>
#include <QApplication>
#include <QDebug>

class CustomDelegate : public QStyledItemDelegate {
public:
    CustomDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {

        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // �����Ҫ����������������Զ�������߼�

        // ���ý�����λ���
        opt.state &= ~QStyle::State_HasFocus;
        return QStyledItemDelegate::paint(painter, opt, index);
    }
};

class CustomProxyStyle : public QProxyStyle {
public:
    CustomProxyStyle(QTableView* view) :_view(view) {}
public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget = nullptr) const override {
        QRect rect = option->rect;
        // ����Ƿ����Ϸ���ص�ԭʼԪ�أ�ע�⣺��ȡ���� Qt ���ڲ�ʵ�֣�
        if (element == PE_IndicatorItemViewItemDrop) {
            if (rect.width() != 0 && rect.height() == 0)
            {
                rect.setLeft(0);
                rect.setRight(_view->width());

                // ����������Ҫ�� DropIndicator �������зָ����ϣ����Ե��� rect �ĸ߶�
                painter->save();
                QColor c = qApp->palette().color(QPalette::Shadow);
                painter->setPen(QPen(c, 3)); // ������ɫΪ��ɫ������Ϊʾ����
                painter->drawLine(rect.topLeft(), rect.topRight());
                painter->restore();
            }
            return;
        }

        // ���ڲ����Ϸ���ص�ԭʼԪ�أ����û���� drawPrimitive ����
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

private:

    QTableView* _view;
};

TableView::TableView(QWidget* parent)
    : QTableView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDefaultDropAction(Qt::MoveAction);
    setDragDropOverwriteMode(false);
    setDropIndicatorShown(true);
    setShowGrid(false);
    setAlternatingRowColors(true);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setItemDelegate(new CustomDelegate(this));
    setStyle(new CustomProxyStyle(this));
}

void TableView::setModel(QAbstractItemModel* model)
{
    QTableView::setModel(model);
    TableModel* tm = dynamic_cast<TableModel*>(model);
    if (tm)
    {
        connect(tm, &TableModel::selectedRowChanged, [this](int row) {
            this->selectRow(row);
            });
    }
}