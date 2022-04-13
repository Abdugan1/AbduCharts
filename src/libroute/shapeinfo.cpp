#include "shapeinfo.h"

ShapeInfo::ShapeInfo(QGraphicsItem *item, ConnectionSide connectionSide)
    : item_(item)
    , connectionSide_(connectionSide)
{

}

QGraphicsItem *ShapeInfo::item() const
{
    return item_;
}

void ShapeInfo::setItem(QGraphicsItem *item)
{
    item_ = item;
}

ConnectionSide ShapeInfo::connectionSide() const
{
    return connectionSide_;
}

void ShapeInfo::setConnectionSide(const ConnectionSide &connectionSide)
{
    connectionSide_ = connectionSide;
}
