#include "movetextcommand.h"
#include "editor/items/flowcharttextitem.h"

#include <QDebug>

MoveTextCommand::MoveTextCommand(FlowchartTextItem *textItem, const QPointF &oldPos)
    : textItem_(textItem)
{
    setOldPos(oldPos);
    setNewPos(textItem->pos());
}

void MoveTextCommand::undo()
{
    qDebug() << "MoveTextCommand: undo";
    textItem_->setPos(oldPos());
}

void MoveTextCommand::redo()
{
    qDebug() << "MoveTextCommand: redo";
    textItem_->setPos(newPos());
}
