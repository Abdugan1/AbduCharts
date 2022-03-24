#ifndef CONNECTORITEM_H
#define CONNECTORITEM_H

#include "items/shapeitembase.h"

#include "libroute/shapeinfo.h"

class ConnectorItem : public ShapeItemBase
{
public:
    explicit ConnectorItem(QGraphicsItem* parent = nullptr);
    ~ConnectorItem();

    QRectF boundingRect() const override;

    QRectF contentRect() const override;

    QPainterPath shape() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setStartShapeInfo(const ShapeInfo &startShapeInfo);

    void setEndShapeInfo(const ShapeInfo &endShapeInfo);

    QGraphicsItem* startItem() const;
    QGraphicsItem* endItem() const;

    void updateConnectionPath();

private:
    QPainterPath linesToPath() const;

private:
    ShapeInfo startShapeInfo_;
    ShapeInfo endShapeInfo_;

    QList<QLineF> connectionPath_;
};

#endif // CONNECTORITEM_H
