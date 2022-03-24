#include "ui/itembutton.h"
#include "ui/shapeitemdrag.h"
#include "editor/items/flowchartshapeitem.h"

#include <QMouseEvent>
#include <QApplication>
#include <QPainter>

QPixmap getPixmapFromShapeItem(const FlowchartShapeItem& shapeItem)
{
    QPixmap pixmap(FlowchartShapeItem::DefaultSize::Width,
                   FlowchartShapeItem::DefaultSize::Height);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // Because shape items center is 0, 0 point
    painter.translate(pixmap.width() / 2, pixmap.height() / 2);

    painter.setPen(QPen(Qt::black, 4));
    painter.setBrush(Qt::white);
    painter.drawPath(shapeItem.shape());

    return pixmap;
}

ItemButton::ItemButton(const FlowchartShapeItem &shapeItem, QWidget *parent)
    : QToolButton(parent)
    , figureType_(shapeItem.figureType())
{
    int size = 48;
    setFixedSize(size, size);
    setIconSize({size, size});
    setIcon(getPixmapFromShapeItem(shapeItem));
}

void ItemButton::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
        dragStartPosition_ = event->pos();
}

void ItemButton::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
        return;
    if ((event->pos() - dragStartPosition_).manhattanLength()
            < QApplication::startDragDistance())
        return;

    ShapeItemDrag* drag = new ShapeItemDrag(this);
    drag->setFigureType(figureType_);
    QPixmap pixmap = icon().pixmap(iconSize());
    drag->setPixmap(pixmap);
    drag->setHotSpot(pixmap.rect().center());
    drag->exec(Qt::CopyAction);
}
