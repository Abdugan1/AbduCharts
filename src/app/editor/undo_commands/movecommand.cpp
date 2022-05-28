#include "movecommand.h"
#include "moveshapecommand.h"
#include "movetextcommand.h"

#include <QDebug>

MoveCommand::MoveCommand(QUndoCommand *parent)
    : QUndoCommand(parent)
{
}

MoveCommand *MoveCommand::fromShapeItem(FlowchartShapeItem *shapeItem, const QPointF &oldPos)
{
    return new MoveShapeCommand(shapeItem, oldPos);
}

MoveCommand *MoveCommand::fromTextItem(FlowchartTextItem *textItem, const QPointF &oldPos)
{
    return new MoveTextCommand(textItem, oldPos);
}

QPointF MoveCommand::oldPos() const
{
    return oldPos_;
}

void MoveCommand::setOldPos(QPointF newOldPos)
{
    oldPos_ = newOldPos;
}

QPointF MoveCommand::newPos() const
{
    return newPos_;
}

void MoveCommand::setNewPos(QPointF newNewPos)
{
    newPos_ = newNewPos;
}

//void MoveCommand::undo()
//{
//    qDebug() << "MoveCommand: undo";
//    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
//    item_->setPos(oldPos_);
//    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
//}

//void MoveCommand::redo()
//{
//    qDebug() << "MoveCommand: redo";
//    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
//    item_->setPos(newPos_);
//    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
//}
