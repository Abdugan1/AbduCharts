#include "processshapeitem.h"

ProcessShapeItem::ProcessShapeItem(QGraphicsItem *parent)
    : FlowchartShapeItem(parent)
{
    QRectF rect(-Width / 2, -Height / 2, Width, Height);
    QPainterPath shape;
    shape.addRect(rect);

    initByShape(shape);
}

int ProcessShapeItem::type() const
{
    return Type;
}
