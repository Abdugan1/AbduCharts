#include "resizehandle.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

using PositionFlags = ResizeHandle::PositionFlags;

constexpr qreal Width  = 8.0f;
constexpr qreal Height = 8.0f;

ResizeHandle::ResizeHandle(PositionFlags positionFlags, QGraphicsItem *parent)
    : ItemBase(parent)
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

QPointF ResizeHandle::prevPos() const
{
    return prevPos_;
}

void ResizeHandle::setPrevPos(QPointF newPrevPos)
{
    prevPos_ = newPrevPos;
}

void ResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF oldPos = scenePos();
    QPointF newPos = snapToGrid(event->scenePos(), 20);
    newPos = restrictedPosition(oldPos, newPos);
    if (newPos != oldPos) {
        qreal dx = newPos.x() - oldPos.x();
        qreal dy = newPos.y() - oldPos.y();
//        qDebug() << "oldPos:" << oldPos;
//        qDebug() << "newPos:" << newPos;
//        qDebug() << "dx:" << dx;
//        qDebug() << "dy:" << dy;
        emit moved(this, dx, dy);
    }
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


















