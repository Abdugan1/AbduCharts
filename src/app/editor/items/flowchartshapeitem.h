#ifndef FLOWCHARTSHAPEITEM_H
#define FLOWCHARTSHAPEITEM_H

#include "editor/items/shapeitembase.h"
#include "editor/resizehandle.h"

#include <memory>

class ItemResizer;
class FlowchartShapesTextItem;
class ConnectorPoint;
enum class ConnectionSide;
class ConnectorItemManager;
class ConnectorItem;

class QTextCharFormat;

class FlowchartShapeItem : public QObject, public ShapeItemBase
{
    Q_OBJECT
public:
    enum { Type = UserType + 2 };

    enum DefaultSize {
        Width  = 200,
        Height = 80
    };

    explicit FlowchartShapeItem(QGraphicsItem* parent = nullptr);
    virtual ~FlowchartShapeItem() = 0;

    QRectF boundingRect() const override;
    QRectF contentRect()  const override;

    int type() const override;

    QPainterPath shape() const override;
    void setShape(const QPainterPath& shape);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setText(const QString& text);

    FlowchartShapesTextItem *textItem() const;

    void addConnectorItem(ConnectorItem* connectorItem);

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

    void connectorPointPressed(ConnectorPoint* point);

private slots:
    void onResizeHandleMoved(ResizeHandle* resizeHandle, qreal dx, qreal dy);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event)       override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)     override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void addResizeHandle(ResizeHandle::PositionFlags positionFlags);
    void updateResizeHandlesPositions();

    void addConnectorPoint(ConnectionSide connectionSide);
    void updateConnectorPointsPositions();

    void initByShape(const QPainterPath& shape);

private:
    bool textEditingEnabled() const;

private:
    QList<ResizeHandle*> resizeHandles_;
    QList<ConnectorPoint*> connectorPoints_;

    QPainterPath shape_;
    ItemResizer* resizer_ = nullptr;

    FlowchartShapesTextItem* textItem_ = nullptr;

   ConnectorItemManager* connectorItemManager_ = nullptr;
};

#endif // FLOWCHARTSHAPEITEM_H
