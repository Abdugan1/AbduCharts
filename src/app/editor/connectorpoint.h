#ifndef CONNECTORPOINT_H
#define CONNECTORPOINT_H

#include "editor/items/shapeitembase.h"

#include "libroute/shapeinfo.h"

class ConnectorPoint : public QObject, public ShapeItemBase
{
    Q_OBJECT
public:
    explicit ConnectorPoint(ConnectionSide side, QGraphicsItem* parent);

    QRectF boundingRect() const override;
    QRectF contentRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    ConnectionSide connectionSide() const;
    QGraphicsItem *item() const;

    ShapeInfo shapeInfo() const;

signals:
    void pressed(ConnectorPoint* handle);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    ShapeInfo shapeInfo_;
};

#endif // CONNECTORPOINT_H
