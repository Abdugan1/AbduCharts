#include "ui/itembutton.h"
#include "ui/shapeitemdrag.h"
#include "editor/items/flowchartshapeitem.h"
#include "editor/items/shapeitemcreator.h"

#include <QMouseEvent>
#include <QApplication>
#include <QPainter>

QPixmap getPixmapFromShapeItem(int itemType)
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
    FlowchartShapeItem* item = ShapeItemCreator::createShape(itemType);
    painter.drawPath(item->shape());

    return pixmap;
}

ItemButton::ItemButton(int itemType, QWidget *parent)
    : QToolButton(parent)
    , itemType_(itemType)
{
    int size = 48;
    setFixedSize(size, size);
    setIconSize({size, size});
    setIcon(getPixmapFromShapeItem(itemType));
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
    drag->setItemType(itemType_);
    QPixmap pixmap = icon().pixmap(iconSize());
    drag->setPixmap(pixmap);
    drag->setHotSpot(pixmap.rect().center());
    drag->exec(Qt::CopyAction);
}
