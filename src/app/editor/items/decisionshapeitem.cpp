#include "decisionshapeitem.h"

DecisionShapeItem::DecisionShapeItem(QGraphicsItem *parent)
    : FlowchartShapeItem(parent)
{
    QRectF rect(-Width / 2, -Height / 2, Width, Height);
    QPolygonF polygon;
    polygon << QPointF(rect.left(),
                       rect.top()  + rect.height() / 2)
            << QPointF(rect.left() + rect.width()  / 2,
                       rect.top())
            << QPointF(rect.right(),
                       rect.top()  + rect.height() / 2)
            << QPointF(rect.left() + rect.width()  / 2,
                       rect.bottom())
            << QPointF(rect.left(),
                       rect.top()  + rect.height() / 2);
    QPainterPath shape;
    shape.addPolygon(polygon);

    initByShape(shape);
}

int DecisionShapeItem::type() const
{
    return Type;
}
