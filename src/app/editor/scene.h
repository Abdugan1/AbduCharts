#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class FlowchartShapeItem;
class GuideLine;
class PositionLine;
class FlowchartTextItem;
class ConnectorItem;
class ConnectorPoint;

class QTextCharFormat;
class QTextBlockFormat;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(const QRectF& sceneRect, QObject* parent = nullptr);
    ~Scene();

    void addShapeItem(FlowchartShapeItem* item);
    void addTextItem(FlowchartTextItem* item);
    void addConnectorItem(ConnectorItem* item);

    void removeShapeItem(FlowchartShapeItem* item);
    void removeTextItem(FlowchartTextItem* item);
    void removeConnectorItem(ConnectorItem* item);

signals:
    void itemSelected(FlowchartShapeItem* item);
    void itemLostSelection(FlowchartShapeItem* item);
    void itemMoved(FlowchartShapeItem* item);

    void itemMovedAndReleased(QGraphicsItem* item, const QPointF& oldPos);

    void currentCharFormatChanged(const QTextCharFormat& format);
    void currentBlockFormatChanged(const QTextBlockFormat& format);
    void switchedToAnotherTextItem(FlowchartTextItem* textItem);

    void newConnectorItemAdded(ConnectorItem* connectorItem);

    void resizeCompleted(FlowchartShapeItem* resizedItem,
                         const QRectF& oldRect,
                         const QRectF& newRect,
                         const QPointF& posBeforeResize,
                         const QPointF& posAfterResize);

public slots:
    void applyCharFormatOnCurrentTextItem(const QTextCharFormat& format);
    void applyBlockFormatOnCurrentTextItem(const QTextBlockFormat& format);

private slots:
    void onItemMoved(FlowchartShapeItem* movedItem);

    void onItemResizedByHands(const FlowchartShapeItem* resizedItem,
                              const QRectF& oldRect,
                              const QRectF& currentRect);

    void savePressedItemInfo(QGraphicsItem* item);

    void onItemReleased();

    void bringToFront(FlowchartShapeItem* item);

    void deleteAllGuidelines();

    void doConnectorItem(const ConnectorPoint* handle);

    void emitResizeInfoToViewUndoStack();

private:
    void addGuideLine(GuideLine* positionLine);

    void connectSignalsOfShapeItem(FlowchartShapeItem* item);
    void connectSignalsOfTextItem(FlowchartTextItem* textItem);

    void addPositionLines(const FlowchartShapeItem* movedItem);

    void addSizeLines(const FlowchartShapeItem* resizedItem,
                      const QRectF& oldRect,
                      const QRectF& currentRect);

private:
    QList<FlowchartShapeItem*> flowchartShapeItems_;
    QList<FlowchartTextItem*> textItems_;
    QList<GuideLine*> guideLines_;

    QGraphicsItem* clickedItem_ = nullptr;
    QPointF clickedItemOldPos_;

    ConnectorItem* lastConnectorItem_ = nullptr;
};

#endif // SCENE_H
