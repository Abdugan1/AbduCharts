#include "deletecommand.h"
#include "editor/items/flowchartshapeitem.h"
#include "editor/items/flowcharttextitem.h"
#include "editor/scene.h"


DeleteCommand *DeleteCommand::fromShapeItem(FlowchartShapeItem *shapeItem, Scene *scene)
{
    DeleteCommand* deleteCommand = new DeleteCommand;
    deleteCommand->shapeItem_ = shapeItem;
    deleteCommand->scene_ = scene;

    shapeItem->setSelected(false);

    return deleteCommand;
}

DeleteCommand *DeleteCommand::fromTextItem(FlowchartTextItem *textItem, Scene *scene)
{
    DeleteCommand* deleteCommand = new DeleteCommand;
    deleteCommand->textItem_ = textItem;
    deleteCommand->scene_ = scene;

    textItem->setSelected(false);

    return deleteCommand;
}

void DeleteCommand::undo()
{
    if (shapeItem_) {
        scene_->addItem(shapeItem_);
    } else {
        scene_->addItem(textItem_);
    }
}

void DeleteCommand::redo()
{
    if (shapeItem_) {
        scene_->removeItem(shapeItem_);
    } else {
        scene_->removeItem(textItem_);
    }
}
