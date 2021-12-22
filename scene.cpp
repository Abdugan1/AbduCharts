#include "scene.h"
#include "flowchartitems.h"
#include "textitem.h"
#include "guideline.h"

#include <QTextCharFormat>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
{

}

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

void Scene::applyFormatOnCurrentTextItem(const QTextCharFormat &format)
{
    QGraphicsItem* focusItem = this->focusItem();
    if (auto textItem = qgraphicsitem_cast<FlowchartTextItem*>(focusItem)) {
        textItem->mergeTextFormat(format);
    }
}

void Scene::onItemMoved(FlowchartShapeItem *movedItem)
{
    if (selectedItems().count() > 1)
        return;

    deleteAllGuidelines();

    QPointF movedItemPos = movedItem->pos();

    QPointF verticalBegin = movedItemPos;
    QPointF verticalEnd   = movedItemPos;

    QPointF horizontalBegin = movedItemPos;
    QPointF horizontalEnd   = movedItemPos;
    for (const auto& item : qAsConst(flowchartShapeItems_)) {
        QPointF currentItemPos = item->pos();

        // x
        if (currentItemPos.x() == movedItemPos.x()) {
            verticalBegin.setY(qMin(currentItemPos.y(), verticalBegin.y()));
            verticalEnd.setY(qMax(currentItemPos.y(), verticalEnd.y()));
        }

        //y
        if (currentItemPos.y() == movedItemPos.y()) {
            horizontalBegin.setX(qMin(currentItemPos.x(), horizontalBegin.x()));
            horizontalEnd.setX(qMax(currentItemPos.x(), horizontalEnd.x()));
        }
    }

    if (verticalBegin.y() != movedItemPos.y()) {
        addPositionLine(new PositionLine({verticalBegin, verticalEnd}));
    } else if (verticalEnd.y() != movedItemPos.y()) {
        addPositionLine(new PositionLine({verticalEnd, verticalBegin}));
    }

    if (horizontalBegin.x() != movedItemPos.x()) {
        addPositionLine(new PositionLine({horizontalBegin, horizontalEnd}));
    } else if (horizontalEnd.x() != movedItemPos.x()) {
        addPositionLine(new PositionLine({horizontalEnd, horizontalBegin}));
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

void Scene::addPositionLine(PositionLine *positionLine)
{
    QGraphicsScene::addItem(positionLine);
    guideLines_.append(positionLine);
}

void Scene::connectSignalsOfShapeItem(FlowchartShapeItem *item)
{
    connect(item, &FlowchartShapeItem::moved,           this, &Scene::onItemMoved);
    connect(item, &FlowchartShapeItem::selected,        this, &Scene::bringToFront);
    connect(item, &FlowchartShapeItem::released,        this, &Scene::deleteAllGuidelines);
    connect(item, &FlowchartShapeItem::selected,        this, &Scene::itemSelected);
    connect(item, &FlowchartShapeItem::moved,           this, &Scene::itemMoved);
    connect(item, &FlowchartShapeItem::lostSelection,   this, &Scene::itemLostSelection);
}

void Scene::connectSignalsOfTextItem(FlowchartTextItem *textItem)
{
    connect(textItem, &FlowchartTextItem::currentCharFormatChanged,
            this,     &Scene::currentCharFormatChanged);

    connect(textItem, &FlowchartTextItem::enabled,
            this,     &Scene::switchedToAnotherTextItem);
}


