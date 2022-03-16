#include "editor/resizehandle.h"
#include "editor/grid.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
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
    setCursorByResizeHandleType();
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

void ResizeHandle::setCursorByResizeHandleType()
{
    switch (positionFlags_) {
    case ResizeHandle::TopLeft:
    case ResizeHandle::BottomRight:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case ResizeHandle::Top:
    case ResizeHandle::Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case ResizeHandle::TopRight:
    case ResizeHandle::BottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case ResizeHandle::Left:
    case ResizeHandle::Right:
        setCursor(Qt::SizeHorCursor);
        break;
    }
}


















