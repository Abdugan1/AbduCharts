#include "editor/items/shapeitembase.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

ShapeItemBase::ShapeItemBase(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
}

ShapeItemBase::~ShapeItemBase()
{
    qDebug() << "ShapeItemBase: Destructor";
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
