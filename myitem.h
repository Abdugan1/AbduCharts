#ifndef MYITEM_H
#define MYITEM_H

#include "itembase.h"

class ResizeHandle;
class ItemResizer;
class MyTextItem;

class MyItem : public QObject, public ItemBase
{
    Q_OBJECT
public:
    explicit MyItem(QGraphicsItem* parent = nullptr);
    ~MyItem();

    QRectF boundingRect() const override;
    QRectF contentRect() const override;
    QPainterPath shape() const override;
    void setShape(const QPainterPath& shape);
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

signals:
    void moved(MyItem* item);
    void resized();
    void selected(MyItem* item);
    void lostSelection(MyItem* item);
    void released();

private slots:
    void onResizeHandleMoved(ResizeHandle* resizeHandle, qreal dx, qreal dy);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event)       override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)     override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)        override;

private:
    void updateResizeHandlesPositions() const;

    bool textEditingEnabled() const;
    void simulateClickOnTextItem(const QPointF& clickedPos);
    void selectTextInTextItemOnMoving(const QPointF& movedPos);
    int getTextCursorPositionByMousePos(const QPointF& mousePos) const;

private:
    QList<ResizeHandle*> resizeHandles_;
    QPainterPath shape_;
    ItemResizer* resizer_ = nullptr;

    MyTextItem* textItem_ = nullptr;
};

#endif // MYITEM_H
