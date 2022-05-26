#include "deleteshapecommand.h"
#include "editor/items/flowchartshapeitem.h"
#include "editor/scene.h"

#include <QDebug>

DeleteShapeCommand::DeleteShapeCommand(FlowchartShapeItem *shapeItem, Scene *scene)
{
    shapeItem_ = shapeItem;
    shapeItem->setSelected(false);

    setScene(scene);
}

void DeleteShapeCommand::undo()
{
    qDebug() << "DeleteShapeCommand: undo";
    scene()->addShapeItem(shapeItem_);
}

void DeleteShapeCommand::redo()
{
    qDebug() << "DeleteShapeCommand: redo";
    scene()->removeShapeItem(shapeItem_);
}
