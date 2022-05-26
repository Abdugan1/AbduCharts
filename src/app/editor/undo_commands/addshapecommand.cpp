#include "addshapecommand.h"
#include "editor/items/shapeitemcreator.h"
#include "editor/items/flowchartshapeitem.h"
#include "editor/scene.h"

#include <QDebug>

AddShapeCommand::AddShapeCommand(int itemType, const QPointF &pos, Scene *scene)
{
    shapeItem_ = ShapeItemCreator::createShape(itemType);
    shapeItem_->setSelected(true);

    setInitialPos(pos);
    setScene(scene);

}

void AddShapeCommand::undo()
{
    qDebug() << "AddShapeCommand: undo";
    scene()->removeShapeItem(shapeItem_);
}

void AddShapeCommand::redo()
{
    qDebug() << "AddShapeCommand: redo";
    shapeItem_->setPos(initialPos());
    scene()->addShapeItem(shapeItem_);
}
