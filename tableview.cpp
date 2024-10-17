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

        // 如果需要，可以在这里添加自定义绘制逻辑

        // 禁用焦点矩形绘制
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
        // 检查是否是拖放相关的原始元素（注意：这取决于 Qt 的内部实现）
        if (element == PE_IndicatorItemViewItemDrop) {
            if (rect.width() != 0 && rect.height() == 0)
            {
                rect.setLeft(0);
                rect.setRight(_view->width());

                // 假设我们想要将 DropIndicator 绘制在行分隔符上，可以调整 rect 的高度
                painter->save();
                QColor c = qApp->palette().color(QPalette::Shadow);
                painter->setPen(QPen(c, 3)); // 设置颜色为红色（仅作为示例）
                painter->drawLine(rect.topLeft(), rect.topRight());
                painter->restore();
            }
            return;
        }

        // 对于不是拖放相关的原始元素，调用基类的 drawPrimitive 方法
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