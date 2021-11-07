#include "itembase.h"

#include <QGraphicsSceneMouseEvent>

ItemBase::ItemBase(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
}

ItemBase::ItemBase(const Data &data, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    applyData(data);
}

const QMarginsF &ItemBase::margins() const
{
    return margins_;
}

void ItemBase::setMargins(const QMarginsF &newMargins)
{
    margins_ = newMargins;
}

void ItemBase::applyData(const Data &data)
{
    setPos(data.x, data.y);
    margins_ = data.margins;
}

QPointF snapToGrid(const QPointF &point, int gridSize)
{
    return QPointF(qRound(point.x() / gridSize) * gridSize,
                   qRound(point.y() / gridSize) * gridSize);
}
