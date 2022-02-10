#ifndef FLOWCHARTITEMS_H
#define FLOWCHARTITEMS_H

#include "shapeitembase.h"
#include "resizehandle.h"

class ItemResizer;
class FlowchartShapesTextItem;

class QTextCharFormat;

class FlowchartShapeItem : public QObject, public ShapeItemBase
{
    Q_OBJECT
public:
    enum DefaultSize {
        Width  = 200,
        Height = 80
    };

    explicit FlowchartShapeItem(QGraphicsItem* parent = nullptr);
    ~FlowchartShapeItem();

    QRectF boundingRect() const override;
    QRectF contentRect()  const override;

    QPainterPath shape() const override;
    void setShape(const QPainterPath& shape);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual QString figureType() const = 0;

    void setText(const QString& text);

    FlowchartShapesTextItem *textItem() const;

signals:
    void moved(FlowchartShapeItem* item);

    void resizedByHands(FlowchartShapeItem* item,
                        const QRectF& oldRect,
                        const QRectF& currentRect);

    void selected(FlowchartShapeItem* item);
    void lostSelection(FlowchartShapeItem* item);
    void pressed(FlowchartShapeItem* item);
    void released();

    void resizeHandleReleased();

private slots:
    void onResizeHandleMoved(ResizeHandle* resizeHandle, qreal dx, qreal dy);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event)       override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)     override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void addResizeHandle(ResizeHandle::PositionFlags positionFlags);
    void updateResizeHandlesPositions() const;

    void initShape_SetResizerRect_UpdateHandlePositions(const QPainterPath& shape);

private:
    bool textEditingEnabled() const;

private:
    QList<ResizeHandle*> resizeHandles_;
    QPainterPath shape_;
    ItemResizer* resizer_ = nullptr;

    FlowchartShapesTextItem* textItem_ = nullptr;
};

//------------------------------------------------
//                Terminal
//------------------------------------------------

class Terminal : public FlowchartShapeItem
{
    Q_OBJECT
public:
    explicit Terminal(QGraphicsItem* parent = nullptr);
    QString figureType() const override;
};

//------------------------------------------------
//                Process
//------------------------------------------------

class Process : public FlowchartShapeItem
{
    Q_OBJECT
public:
    explicit Process(QGraphicsItem* parent = nullptr);
    QString figureType() const override;
};

//------------------------------------------------
//                Decision
//------------------------------------------------

class Decision : public FlowchartShapeItem
{
    Q_OBJECT
public:
    explicit Decision(QGraphicsItem* parent = nullptr);
    QString figureType() const override;
};

//------------------------------------------------
//                InOut
//------------------------------------------------

class InOut : public FlowchartShapeItem
{
    Q_OBJECT
public:
    explicit InOut(QGraphicsItem* parent = nullptr);
    QString figureType() const override;
};


//------------------------------------------------
//                ShapeCreator
//------------------------------------------------


class ShapeCreator
{
public:
    static FlowchartShapeItem* createShape(const QString& shapeType);
};

#endif // FLOWCHARTITEMS_H
