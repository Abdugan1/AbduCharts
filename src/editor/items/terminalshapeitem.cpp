#include "terminalshapeitem.h"

TerminalShapeItem::TerminalShapeItem(QGraphicsItem *parent)
    : FlowchartShapeItem(parent)
{
    QRectF rect(-Width / 2, -Height / 2, Width, Height);
    QPainterPath shape;
    shape.addRoundedRect(rect, Height / 2, Height / 2);

    initByShape(shape);
}

QString TerminalShapeItem::figureType() const
{
    return "Terminal";
}
