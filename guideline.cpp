#include "guideline.h"

#include <QPainter>
#include <QDebug>

GuideLine::GuideLine(QGraphicsItem *parent)
    : ShapeItemBase(parent)
{
    setZValue(INFINITY);
}

PositionLine::PositionLine(const QLineF &line, QGraphicsItem *parent)
    : GuideLine(parent)
    , line_(line)
{
    setMargins(QMargins(5, 5, 5, 5));
}

PositionLine::~PositionLine()
{
    qDebug() << "PositionLine has left the game";
}

//------------------------------------------------
//                PositionLine
//------------------------------------------------

QRectF PositionLine::boundingRect() const
{
    return contentRect() + margins();
}

void PositionLine::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
    painter->setPen(Qt::red);
    painter->drawRect(boundingRect());

    QPen pen;
    pen.setColor(Qt::blue);
    pen.setStyle(Qt::DashLine);
    pen.setDashPattern(QVector<qreal>({10, 5}));
    painter->setPen(pen);
    painter->drawLine(line_);
}

QRectF PositionLine::contentRect() const
{
    return QRectF(line_.x1(), line_.y1(),
                  line_.x2() - line_.x1(), line_.y2() - line_.y1())
            .normalized();
}
