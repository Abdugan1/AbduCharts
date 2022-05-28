#include "moveshapecommand.h"
#include "editor/items/flowchartshapeitem.h"

#include <QDebug>

MoveShapeCommand::MoveShapeCommand(FlowchartShapeItem *shapeItem, const QPointF &oldPos)
    : shapeItem_(shapeItem)
{
    setOldPos(oldPos);
    setNewPos(shapeItem->pos());
}

void MoveShapeCommand::undo()
{
    qDebug() << "MoveShapeCommand: undo";
    shapeItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    shapeItem_->setPos(oldPos());
    shapeItem_->updateConnectorItems();
    shapeItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void MoveShapeCommand::redo()
{
    qDebug() << "MoveShapeCommand: redo";
    shapeItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    shapeItem_->setPos(newPos());
    shapeItem_->updateConnectorItems();
    shapeItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
