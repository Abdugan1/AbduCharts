#include "movecommand.h"
#include <QGraphicsItem>

#include <QDebug>

MoveCommand::MoveCommand(QGraphicsItem* shapeItem, const QPointF& oldPos)
    : item_(shapeItem)
    , oldPos_(oldPos)
    , newPos_(shapeItem->pos())
{
}

void MoveCommand::undo()
{
    qDebug() << "Move: undo";
    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    item_->setPos(oldPos_);
    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void MoveCommand::redo()
{
    qDebug() << "Move: redo";
    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    item_->setPos(newPos_);
    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
