#include "editor/connectorpoint.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

constexpr qreal Width  = 10.0f;
constexpr qreal Height = 10.0f;

ConnectorPoint::ConnectorPoint(ConnectionSide side, QGraphicsItem *parent)
    : ShapeItemBase(parent)
{
    shapeInfo_.setConnectionSide(side);
    shapeInfo_.setItem(parent);

    setAcceptHoverEvents(true);
    setCursor(Qt::PointingHandCursor);

    const int m = 5;
    setMargins(QMarginsF(m, m, m, m));
}

QRectF ConnectorPoint::boundingRect() const
{
    return contentRect() + margins();
}

QRectF ConnectorPoint::contentRect() const
{
    return QRectF(-Width / 2, -Height / 2, Width, Height);
}

void ConnectorPoint::paint(QPainter *painter,
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

ConnectionSide ConnectorPoint::connectionSide() const
{
    return shapeInfo_.connectionSide();
}

QGraphicsItem *ConnectorPoint::item() const
{
    return shapeInfo_.item();
}

ShapeInfo ConnectorPoint::shapeInfo() const
{
    return shapeInfo_;
}

void ConnectorPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ShapeItemBase::mousePressEvent(event);
    emit pressed(this);
}
