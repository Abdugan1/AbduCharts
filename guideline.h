#ifndef GUIDELINE_H
#define GUIDELINE_H

#include "itembase.h"

class GuideLine : public ShapeItemBase
{
public:
    explicit GuideLine(QGraphicsItem* parent = nullptr);
};


class PositionLine : public GuideLine
{
public:
    explicit PositionLine(const QLineF& line, QGraphicsItem* parent = nullptr);
    ~PositionLine();

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QRectF contentRect() const override;

private:
    QLineF line_;
};

#endif // GUIDELINE_H
