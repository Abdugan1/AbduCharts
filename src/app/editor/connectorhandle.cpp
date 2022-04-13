#include "connectorhandle.h"
#include "grid.h"

#include "libroute/waypoint.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

constexpr qreal Width  = 10.0f;
constexpr qreal Height = 10.0f;

ConnectorHandle::ConnectorHandle(const QLineF &line, int lineIndexInList, QGraphicsItem *parent)
    : ShapeItemBase(parent)
    , lineIndex_(lineIndexInList)
    , moveOrientation_(getOrientationFromLine(line))
{
    setFlag(ItemIsMovable);
}

QRectF ConnectorHandle::boundingRect() const
{
    return QRectF(-Width / 2, -Height / 2, Width, Height);
}

QRectF ConnectorHandle::contentRect() const
{
    return boundingRect() + margins();
}

void ConnectorHandle::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(QPen(Qt::white));
    painter->setBrush(QBrush(QColor(41, 182, 242)));
    painter->drawEllipse(contentRect());
}

void ConnectorHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF oldPos = scenePos();
    QPointF newPos = Grid::enabled() ? Grid::snapToGrid(event->scenePos())
                                     : event->scenePos();
    newPos = restrictedPosition(oldPos, newPos);
    if (newPos != oldPos) {
        qDebug() << "newPos:" << newPos;
        emit moved(this, newPos);
    }
}

MoveOrientation ConnectorHandle::getOrientationFromLine(const QLineF &line) const
{
    return (line.p1().x() == line.p2().x() ? MoveOrientation::LeftRight: MoveOrientation::UpDown);
}

QPointF ConnectorHandle::restrictedPosition(const QPointF &oldPos, const QPointF &newPos)
{
    QPointF output = oldPos;

    if (moveOrientation_ == MoveOrientation::UpDown)
        output.setY(newPos.y());
    else
        output.setX(newPos.x());

    return output;
}

MoveOrientation ConnectorHandle::moveOrientation() const
{
    return moveOrientation_;
}

int ConnectorHandle::lineIndex() const
{
    return lineIndex_;
}

void ConnectorHandle::setLineIndex(int lineIndex)
{
    lineIndex_ = lineIndex;
}
