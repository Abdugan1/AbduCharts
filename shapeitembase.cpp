#include "shapeitembase.h"

#include <QGraphicsSceneMouseEvent>

ShapeItemBase::ShapeItemBase(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
}

ShapeItemBase::ShapeItemBase(const Data &data, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    applyData(data);
}

int ShapeItemBase::type() const
{
    return Type;
}

const QMarginsF &ShapeItemBase::margins() const
{
    return margins_;
}

void ShapeItemBase::setMargins(const QMarginsF &newMargins)
{
    margins_ = newMargins;
}

void ShapeItemBase::applyData(const Data &data)
{
    setPos(data.x, data.y);
    margins_ = data.margins;
}

QPointF snapToGrid(const QPointF &point, int gridSize)
{
    return QPointF(qRound(point.x() / gridSize) * gridSize,
                   qRound(point.y() / gridSize) * gridSize);
}
