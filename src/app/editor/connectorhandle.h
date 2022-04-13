#ifndef CONNECTORHANDLE_H
#define CONNECTORHANDLE_H

#include "editor/items/shapeitembase.h"

enum class MoveOrientation;

class ConnectorHandle : public QObject, public ShapeItemBase
{
    Q_OBJECT
public:
    explicit ConnectorHandle(const QLineF& line, int lineIndexInList, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    QRectF contentRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    int lineIndex() const;
    void setLineIndex(int lineIndex);

    MoveOrientation moveOrientation() const;

signals:
    void moved(ConnectorHandle* handle, const QPointF& newPos);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    MoveOrientation getOrientationFromLine(const QLineF& line) const;
    QPointF restrictedPosition(const QPointF& oldPos, const QPointF& newPos);

private:
    int lineIndex_ = 0;
    MoveOrientation moveOrientation_;
};

#endif // CONNECTORHANDLE_H
