#include "editor/scene.h"
#include "editor/items/flowchartshapeitems.h"
#include "editor/items/flowcharttextitems.h"
#include "editor/guidelines.h"
#include "editor/connectorpoint.h"
#include "editor/connectoritem.h"

#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

Scene::Scene(const QRectF &sceneRect, QObject *parent)
    : QGraphicsScene(sceneRect, parent)
{

}

Scene::~Scene()
{
    qDebug() << "Scene: Destructor";
}

void Scene::addShapeItem(FlowchartShapeItem *item)
{
    QGraphicsScene::addItem(item);
    flowchartShapeItems_.append(item);

    connectSignalsOfShapeItem(item);
    connectSignalsOfTextItem(item->textItem());
}

void Scene::addTextItem(FlowchartTextItem *item)
{
    QGraphicsScene::addItem(item);
    textItems_.append(item);

    connectSignalsOfTextItem(item);
}

void Scene::addConnectorItem(ConnectorItem *item)
{
    QGraphicsScene::addItem(item);
}

void Scene::removeShapeItem(FlowchartShapeItem *item)
{
    QGraphicsScene::removeItem(item);
    flowchartShapeItems_.removeAll(item);
}

void Scene::removeTextItem(FlowchartTextItem *item)
{
    QGraphicsScene::removeItem(item);
    textItems_.removeAll(item);
}

void Scene::removeConnectorItem(ConnectorItem *item)
{
    QGraphicsScene::removeItem(item);
}

void Scene::applyCharFormatOnCurrentTextItem(const QTextCharFormat &format)
{
    QGraphicsItem* focusItem = this->focusItem();
    if (auto textItem = dynamic_cast<FlowchartTextItem*>(focusItem)) {
        textItem->mergeCharFormat(format);
    }
}

void Scene::applyBlockFormatOnCurrentTextItem(const QTextBlockFormat &format)
{
    QGraphicsItem* focusItem = this->focusItem();
    if (auto textItem = dynamic_cast<FlowchartTextItem*>(focusItem)) {
        textItem->setBlockFormat(format);
    }
}

void Scene::onItemMoved(FlowchartShapeItem *movedItem)
{
    if (selectedItems().count() > 1)
        return;

    deleteAllGuidelines();
    addPositionLines(movedItem);
}

void Scene::onItemResizedByHands(const FlowchartShapeItem *resizedItem,
                          const QRectF &oldRect,
                          const QRectF &currentRect)
{
    deleteAllGuidelines();
    addSizeLines(resizedItem, oldRect, currentRect);
}

void Scene::savePressedItemInfo(QGraphicsItem *item)
{
    clickedItem_ = item;
    clickedItemOldPos_ = item->pos();
}

void Scene::onItemReleased()
{
    if (clickedItem_ && clickedItemOldPos_ != clickedItem_->pos()) {
        emit itemMovedAndReleased(clickedItem_, clickedItemOldPos_);
    }
}

void Scene::bringToFront(FlowchartShapeItem *item)
{
    qreal zValue = item->zValue();
    for (const auto i : qAsConst(flowchartShapeItems_)) {
        zValue = qMax(zValue, i->zValue());
    }
    item->setZValue(zValue + 0.1);
}

void Scene::deleteAllGuidelines()
{
    for (auto guideLine : guideLines_) {
        QGraphicsScene::removeItem(guideLine);
        delete guideLine;
    }
    guideLines_.clear();
}

void Scene::doConnectorItem(const ConnectorPoint *handle)
{
    if (!lastConnectorItem_) {
        lastConnectorItem_ = new ConnectorItem;
        lastConnectorItem_->setStartShapeInfo(handle->shapeInfo());
        static_cast<FlowchartShapeItem*>(handle->item())->addConnectorItem(lastConnectorItem_);
    } else {
        lastConnectorItem_->setEndShapeInfo(handle->shapeInfo());
        lastConnectorItem_->updateConnectionPath();
        static_cast<FlowchartShapeItem*>(handle->item())->addConnectorItem(lastConnectorItem_);

        emit newConnectorItemAdded(lastConnectorItem_);

        lastConnectorItem_ = nullptr;
    }
}

void Scene::emitResizeInfoToViewUndoStack()
{
    auto shapeItem = qobject_cast<FlowchartShapeItem*>(sender());
    QRectF oldRect = shapeItem->contentRectBeforeResize();
    QRectF newRect = shapeItem->contentRect();
    QPointF posBeforeResize = shapeItem->posBeforeResize();
    QPointF posAfterResize = shapeItem->pos();
    emit resizeCompleted(shapeItem, oldRect, newRect, posBeforeResize, posAfterResize);
}

void Scene::addGuideLine(GuideLine *positionLine)
{
    QGraphicsScene::addItem(positionLine);
    guideLines_.append(positionLine);
}

void Scene::connectSignalsOfShapeItem(FlowchartShapeItem *item)
{
    connect(item, &FlowchartShapeItem::moved,                  this, &Scene::onItemMoved);
    connect(item, &FlowchartShapeItem::resizedByUser,          this, &Scene::onItemResizedByHands);
    connect(item, &FlowchartShapeItem::selected,               this, &Scene::bringToFront);
    connect(item, &FlowchartShapeItem::pressed,                this, &Scene::savePressedItemInfo);
    connect(item, &FlowchartShapeItem::released,               this, &Scene::deleteAllGuidelines);
    connect(item, &FlowchartShapeItem::released,               this, &Scene::onItemReleased);
    connect(item, &FlowchartShapeItem::selected,               this, &Scene::itemSelected);
    connect(item, &FlowchartShapeItem::moved,                  this, &Scene::itemMoved);
    connect(item, &FlowchartShapeItem::lostSelection,          this, &Scene::itemLostSelection);
    connect(item, &FlowchartShapeItem::resizeHandleReleased,   this, &Scene::deleteAllGuidelines);
    connect(item, &FlowchartShapeItem::resizeHandleReleased,   this, &Scene::emitResizeInfoToViewUndoStack);
    connect(item, &FlowchartShapeItem::connectorPointPressed,  this, &Scene::doConnectorItem);
}

void Scene::connectSignalsOfTextItem(FlowchartTextItem *textItem)
{
    connect(textItem, &FlowchartTextItem::currentCharFormatChanged,
            this,     &Scene::currentCharFormatChanged);

    connect(textItem, &FlowchartTextItem::currentBlockFormatChanged,
            this,     &Scene::currentBlockFormatChanged);

    connect(textItem, &FlowchartTextItem::enabled,
            this,     &Scene::switchedToAnotherTextItem);

    connect(textItem, &FlowchartTextItem::pressed,
            this,     &Scene::savePressedItemInfo);

    connect(textItem, &FlowchartTextItem::released,
            this,     &Scene::onItemReleased);
}

void Scene::addPositionLines(const FlowchartShapeItem *movedItem)
{
    QList<PositionLine*> positionLines;
    positionLines.append(GuideLineCreator::createLeftPositionLines(movedItem,   flowchartShapeItems_));
    positionLines.append(GuideLineCreator::createRightPositionLines(movedItem,  flowchartShapeItems_));
    positionLines.append(GuideLineCreator::createTopPositionLines(movedItem,    flowchartShapeItems_));
    positionLines.append(GuideLineCreator::createBottomPositionLines(movedItem, flowchartShapeItems_));
    positionLines.append(GuideLineCreator::createCenterPositionLines(movedItem, flowchartShapeItems_));

    for (auto positionLine : qAsConst(positionLines))
        addGuideLine(positionLine);
}

void Scene::addSizeLines(const FlowchartShapeItem *resizedItem,
                         const QRectF &oldRect,
                         const QRectF &currentRect)
{
    QList<SizeLine*> sizeLines = GuideLineCreator::createSizeLine(resizedItem,
                                                                  oldRect,
                                                                  currentRect,
                                                                  flowchartShapeItems_);

    for (auto sizeLine : qAsConst(sizeLines))
        addGuideLine(sizeLine);
}

