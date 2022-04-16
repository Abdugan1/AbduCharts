#ifndef CONNECTORITEM_H
#define CONNECTORITEM_H

#include "items/shapeitembase.h"

#include "libroute/shapeinfo.h"

class ConnectorHandle;

class ConnectorItem : public QObject, public ShapeItemBase
{
    Q_OBJECT
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

private slots:
    void onConnectorHandleMoved(ConnectorHandle* handle, const QPointF& pos);

private:
    void deleteConnectorHandles();
    void generateConnectorHandles();

    void updateConnectorHandlesPosition();

    void updateConnectionPathBuMovedHandle(const ConnectorHandle* handle,
                                           const QPointF& movedPos);

    ConnectorHandle *createConnectorHandle(const QLineF& line, int lineIndex);

    bool firstHandleMoved(const ConnectorHandle* handle) const;
    bool lastHandleMoved(const ConnectorHandle* handle) const;

    void addNewLineToConnectionPathBegin();
    void addNewLineToConnectionPathEnd();

private:
    QPainterPath linesToPath() const;
    QPolygonF calculateArrow() const;

    void drawArrowHead(QPainter* painter) const;

private:
    ShapeInfo startShapeInfo_;
    ShapeInfo endShapeInfo_;

    QList<QLineF> connectionPath_;
    QList<ConnectorHandle*> connectorHandles_;
    bool autoPathFinding_ = true;
};

#endif // CONNECTORITEM_H
