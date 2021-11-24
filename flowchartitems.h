#ifndef FLOWCHARTITEMS_H
#define FLOWCHARTITEMS_H

#include "itembase.h"
#include "resizehandle.h"

class ItemResizer;
class FlowchartTextItem;

class FlowchartItem : public QObject, public ItemBase
{
    Q_OBJECT
public:
    enum DefaultSize {
        Width  = 200,
        Height = 80
    };

    explicit FlowchartItem(QGraphicsItem* parent = nullptr);
    ~FlowchartItem();

    QRectF boundingRect() const override;
    QRectF contentRect()  const override;

    QPainterPath shape() const override;
    void setShape(const QPainterPath& shape);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual QString figureType() const = 0;

    void setText(const QString& text);

    void setTextAlignment(Qt::Alignment alignment);

signals:
    void moved(FlowchartItem* item);
    void resized();
    void selected(FlowchartItem* item);
    void lostSelection(FlowchartItem* item);
    void released();

private slots:
    void onResizeHandleMoved(ResizeHandle* resizeHandle, qreal dx, qreal dy);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event)       override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)     override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)        override;

    void addResizeHandle(ResizeHandle::PositionFlags positionFlags);
    void updateResizeHandlesPositions() const;

    void initShape_SetResizerRect_UpdateHandlePositions(const QPainterPath& shape);

private:
    bool textEditingEnabled() const;
    void simulateClickOnTextItem(const QPointF& clickedPos);
    void selectTextInTextItemOnMoving(const QPointF& movedPos);
    int getTextCursorPositionByMousePos(const QPointF& mousePos) const;

private:
    QList<ResizeHandle*> resizeHandles_;
    QPainterPath shape_;
    ItemResizer* resizer_ = nullptr;

    FlowchartTextItem* textItem_ = nullptr;
};

//------------------------------------------------
//                Terminal
//------------------------------------------------

class Terminal : public FlowchartItem
{
    Q_OBJECT
public:
    explicit Terminal(QGraphicsItem* parent = nullptr);
    QString figureType() const override;
};

//------------------------------------------------
//                Process
//------------------------------------------------

class Process : public FlowchartItem
{
    Q_OBJECT
public:
    explicit Process(QGraphicsItem* parent = nullptr);
    QString figureType() const override;
};

//------------------------------------------------
//                Decision
//------------------------------------------------

class Decision : public FlowchartItem
{
    Q_OBJECT
public:
    explicit Decision(QGraphicsItem* parent = nullptr);
    QString figureType() const override;
};

//------------------------------------------------
//                InOut
//------------------------------------------------

class InOut : public FlowchartItem
{
    Q_OBJECT
public:
    explicit InOut(QGraphicsItem* parent = nullptr);
    QString figureType() const override;
};

#endif // FLOWCHARTITEMS_H
