#include "deletetextcommand.h"
#include "editor/items/flowcharttextitem.h"
#include "editor/scene.h"

#include <QDebug>

DeleteTextCommand::DeleteTextCommand(FlowchartTextItem *textItem, Scene *scene)
{
    textItem_ = textItem;
    textItem->setSelected(false);

    setScene(scene);
}

void DeleteTextCommand::undo()
{
    qDebug() << "DeleteTextCommand: undo";
    scene()->addTextItem(textItem_);
}

void DeleteTextCommand::redo()
{
    qDebug() << "DeleteTextCommand: redo";
    scene()->removeTextItem(textItem_);
}
