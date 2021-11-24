#include "scene.h"
#include "flowchartitems.h"
#include "guideline.h"

#include <QDebug>

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
{

}

Scene::Scene(const QRectF &sceneRect, QObject *parent)
    : QGraphicsScene(sceneRect, parent)
{

}

void Scene::addItem(FlowchartItem *item)
{
    QGraphicsScene::addItem(item);
    items_.append(item);
    connect(item, &FlowchartItem::moved,           this, &Scene::onItemMoved);
    connect(item, &FlowchartItem::selected,        this, &Scene::bringToFront);
    connect(item, &FlowchartItem::released,        this, &Scene::deleteAllGuidelines);
    connect(item, &FlowchartItem::selected,        this, &Scene::itemSelected);
    connect(item, &FlowchartItem::moved,           this, &Scene::itemMoved);
    connect(item, &FlowchartItem::lostSelection,   this, &Scene::itemLostSelection);
}

void Scene::removeItem(FlowchartItem *item)
{
    QGraphicsScene::removeItem(item);
    items_.removeAll(item);
    item->deleteLater();
}

void Scene::onItemMoved(FlowchartItem *movedItem)
{
    if (selectedItems().count() > 1)
        return;

    deleteAllGuidelines();

    QPointF movedItemPos = movedItem->pos();

    QPointF verticalBegin = movedItemPos;
    QPointF verticalEnd   = movedItemPos;

    QPointF horizontalBegin = movedItemPos;
    QPointF horizontalEnd   = movedItemPos;
    for (const auto& item : qAsConst(items_)) {
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

void Scene::bringToFront(FlowchartItem *item)
{
    qreal zValue = item->zValue();
    for (const auto i : qAsConst(items_)) {
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


