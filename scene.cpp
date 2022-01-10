#include "scene.h"
#include "flowchartitems.h"
#include "textitem.h"
#include "guidelines.h"

#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

Scene::Scene(const QRectF &sceneRect, QObject *parent)
    : QGraphicsScene(sceneRect, parent)
{

}

void Scene::addItem(FlowchartShapeItem *item)
{
    QGraphicsScene::addItem(item);
    flowchartShapeItems_.append(item);

    connectSignalsOfShapeItem(item);
    connectSignalsOfTextItem(item->textItem());
}

void Scene::addItem(FlowchartTextItem *item)
{
    QGraphicsScene::addItem(item);
    textItems_.append(item);

    connectSignalsOfTextItem(item);
}

void Scene::removeItem(FlowchartShapeItem *item)
{
    QGraphicsScene::removeItem(item);
    flowchartShapeItems_.removeAll(item);
    item->deleteLater();
}

void Scene::applyCharFormatOnCurrentTextItem(const QTextCharFormat &format)
{
    QGraphicsItem* focusItem = this->focusItem();
    if (auto textItem = qgraphicsitem_cast<FlowchartTextItem*>(focusItem)) {
        textItem->mergeCharFormat(format);
    }
}

void Scene::applyBlockFormatOnCurrentTextItem(const QTextBlockFormat &format)
{
    QGraphicsItem* focusItem = this->focusItem();
    if (auto textItem = qgraphicsitem_cast<FlowchartTextItem*>(focusItem)) {
        textItem->setBlockFormat(format);
    }
}

void Scene::onItemMoved(const FlowchartShapeItem *movedItem)
{
    if (selectedItems().count() > 1)
        return;

    deleteAllGuidelines();

    QList<PositionLine*> positionLines;
    positionLines.append(GuideLineCreator::createLeftPositionLines(movedItem,   flowchartShapeItems_));
    positionLines.append(GuideLineCreator::createRightPositionLines(movedItem,  flowchartShapeItems_));
    positionLines.append(GuideLineCreator::createTopPositionLines(movedItem,    flowchartShapeItems_));
    positionLines.append(GuideLineCreator::createBottomPositionLines(movedItem, flowchartShapeItems_));
    positionLines.append(GuideLineCreator::createCenterPositionLines(movedItem, flowchartShapeItems_));

    qDebug() << "position lines count:" << positionLines.count();

    for (auto positionLine : qAsConst(positionLines))
        addGuideLine(positionLine);
}

void Scene::onItemResized(const FlowchartShapeItem *resizedItem,
                          const QRectF &oldRect,
                          const QRectF &currentRect)
{
    deleteAllGuidelines();
    QList<SizeLine*> sizeLines =
            GuideLineCreator::createSizeLine(resizedItem,
                                             oldRect,
                                             currentRect,
                                             flowchartShapeItems_);
    for (auto sizeLine : qAsConst(sizeLines))
        addGuideLine(sizeLine);
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

void Scene::addGuideLine(GuideLine *positionLine)
{
    QGraphicsScene::addItem(positionLine);
    guideLines_.append(positionLine);
}

void Scene::connectSignalsOfShapeItem(FlowchartShapeItem *item)
{
    connect(item, &FlowchartShapeItem::moved,                this, &Scene::onItemMoved);
    connect(item, &FlowchartShapeItem::resized,              this, &Scene::onItemResized);
    connect(item, &FlowchartShapeItem::selected,             this, &Scene::bringToFront);
    connect(item, &FlowchartShapeItem::released,             this, &Scene::deleteAllGuidelines);
    connect(item, &FlowchartShapeItem::selected,             this, &Scene::itemSelected);
    connect(item, &FlowchartShapeItem::moved,                this, &Scene::itemMoved);
    connect(item, &FlowchartShapeItem::lostSelection,        this, &Scene::itemLostSelection);
    connect(item, &FlowchartShapeItem::resizeHandleReleased, this, &Scene::deleteAllGuidelines);
}

void Scene::connectSignalsOfTextItem(FlowchartTextItem *textItem)
{
    connect(textItem, &FlowchartTextItem::currentCharFormatChanged,
            this,     &Scene::currentCharFormatChanged);

    connect(textItem, &FlowchartTextItem::currentBlockFormatChanged,
            this,     &Scene::currentBlockFormatChanged);

    connect(textItem, &FlowchartTextItem::enabled,
            this,     &Scene::switchedToAnotherTextItem);
}


