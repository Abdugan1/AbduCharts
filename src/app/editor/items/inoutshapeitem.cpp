#include "inoutshapeitem.h"

InOutShapeItem::InOutShapeItem(QGraphicsItem *parent)
    : FlowchartShapeItem(parent)
{
    QRectF rect(-Width / 2, -Height / 2, Width, Height);
    QPolygonF polygon;
    polygon << QPointF(rect.left() + Width / 4, rect.top()) << QPointF(rect.topRight())
            << QPointF(rect.right() - Width / 4, rect.bottom()) << rect.bottomLeft()
            << QPointF(rect.left() + Width / 4, rect.top());
    QPainterPath shape;

    shape.addPolygon(polygon);

    initByShape(shape);
}

QString InOutShapeItem::figureType() const
{
    return "Input/Output";
}
