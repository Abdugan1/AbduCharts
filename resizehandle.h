#ifndef RESIZEHANDLE_H
#define RESIZEHANDLE_H

#include "itembase.h"

class ResizeHandle : public QObject, public ItemBase
{
    Q_OBJECT
public:
    enum PositionFlags {
        Top         = 0x1,
        Bottom      = 0x2,
        Left        = 0x4,
        TopLeft     = Top | Left,
        BottomLeft  = Bottom | Left,
        Right       = 0x8,
        TopRight    = Top | Right,
        BottomRight = Bottom | Right
    };

    explicit ResizeHandle(PositionFlags positionFlags, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    QRectF contentRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    PositionFlags positionFlags() const;

signals:
    void moved(ResizeHandle* resizeHandle, qreal dx, qreal dy);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF restrictedPosition(const QPointF& oldPos, const QPointF& newPos);

private:
    PositionFlags positionFlags_;
};

#endif // RESIZEHANDLE_H
