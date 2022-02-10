#include "resizehandle.h"
#include "grid.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

using PositionFlags = ResizeHandle::PositionFlags;

constexpr qreal Width  = 8.0f;
constexpr qreal Height = 8.0f;

ResizeHandle::ResizeHandle(PositionFlags positionFlags, QGraphicsItem *parent)
    : ShapeItemBase(parent)
    , positionFlags_(positionFlags)
{
    setFlags(ItemIsMovable);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF ResizeHandle::boundingRect() const
{
    return contentRect() + margins();
}

QRectF ResizeHandle::contentRect() const
{
    return QRectF(-Width / 2, -Height / 2, Width, Height);
}

void ResizeHandle::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
    painter->setBrush(Qt::lightGray);
    painter->drawEllipse(contentRect());
}

PositionFlags ResizeHandle::positionFlags() const
{
    return positionFlags_;
}

void ResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF oldPos = scenePos();
    QPointF newPos = Grid::enabled() ? Grid::snapToGrid(event->scenePos())
                                     : event->scenePos();
    newPos = restrictedPosition(oldPos, newPos);
    if (newPos != oldPos) {
        qreal dx = newPos.x() - oldPos.x();
        qreal dy = newPos.y() - oldPos.y();
        emit moved(this, dx, dy);
    }
}

void ResizeHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ShapeItemBase::mouseReleaseEvent(event);
    emit released();
}

QPointF ResizeHandle::restrictedPosition(const QPointF &oldPos, const QPointF &newPos)
{
    QPointF output = oldPos;

    if (positionFlags_ & Top || positionFlags_ & Bottom)
        output.setY(newPos.y());

    if (positionFlags_ & Left || positionFlags_ & Right)
        output.setX(newPos.x());

    return output;
}


















