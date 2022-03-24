#include "editor/connectorhandle.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

constexpr qreal Width  = 10.0f;
constexpr qreal Height = 10.0f;

ConnectorHandle::ConnectorHandle(ConnectionSide side, QGraphicsItem *parent)
    : ShapeItemBase(parent)
{
    shapeInfo_.side = side;
    shapeInfo_.item = parent;

    setAcceptHoverEvents(true);
    setCursor(Qt::PointingHandCursor);

    const int m = 5;
    setMargins(QMarginsF(m, m, m, m));
}

QRectF ConnectorHandle::boundingRect() const
{
    return contentRect() + margins();
}

QRectF ConnectorHandle::contentRect() const
{
    return QRectF(-Width / 2, -Height / 2, Width, Height);
}

void ConnectorHandle::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(widget)

    const QRectF innerRect = contentRect();

    if (option->state & QStyle::State_MouseOver) {
        const int m = 3;
        const QRectF outerRect = innerRect + QMarginsF(m, m, m, m);
        painter->setPen(Qt::green);
        painter->drawEllipse(outerRect);
    }

    painter->setPen(QPen(Qt::lightGray));
    painter->setBrush(QBrush(Qt::lightGray));
    painter->drawEllipse(innerRect);
}

ConnectionSide ConnectorHandle::connectionSide() const
{
    return shapeInfo_.side;
}

QGraphicsItem *ConnectorHandle::item() const
{
    return shapeInfo_.item;
}

ShapeInfo ConnectorHandle::shapeInfo() const
{
    return shapeInfo_;
}

void ConnectorHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ShapeItemBase::mousePressEvent(event);
    emit pressed(this);
}
