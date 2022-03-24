#ifndef CONNECTORHANDLE_H
#define CONNECTORHANDLE_H

#include "editor/items/shapeitembase.h"

#include "libroute/shapeinfo.h"

class ConnectorHandle : public QObject, public ShapeItemBase
{
    Q_OBJECT
public:
    explicit ConnectorHandle(ConnectionSide side, QGraphicsItem* parent);

    QRectF boundingRect() const override;
    QRectF contentRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    ConnectionSide connectionSide() const;
    QGraphicsItem *item() const;

    ShapeInfo shapeInfo() const;

signals:
    void pressed(ConnectorHandle* handle);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    ShapeInfo shapeInfo_;
};

#endif // CONNECTORHANDLE_H
