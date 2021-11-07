#ifndef MYITEM_H
#define MYITEM_H

#include "itembase.h"

class ResizeHandle;
class ItemResizer;

class MyItem : public QObject, public ItemBase
{
    Q_OBJECT
public:
    explicit MyItem(QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    QRectF contentRect() const override;
    QPainterPath shape() const override;
    void setShape(const QPainterPath& shape);
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private slots:
    void onResizeHandleMoved(ResizeHandle* resizeHandle, qreal dx, qreal dy);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    void updateResizeHandlesPositions();

private:
    QList<ResizeHandle*> resizeHandles_;
    QPainterPath shape_;
    ItemResizer* resizer_ = nullptr;
};

#endif // MYITEM_H