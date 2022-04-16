#include "addcommand.h"
#include "editor/items/flowchartshapeitem.h"
#include "editor/items/shapeitemcreator.h"
#include "editor/items/textitem.h"
#include "editor/scene.h"

#include <QDebug>

AddCommand *AddCommand::fromShapeItem(int itemType, const QPointF &pos, Scene *scene)
{
    AddCommand* addCommand = new AddCommand;
    addCommand->shapeItem_ = ShapeItemCreator::createShape(itemType);
    addCommand->initialPosition_ = pos;
    addCommand->scene_ = scene;

    addCommand->shapeItem_->setSelected(true);

    return addCommand;
}

AddCommand *AddCommand::fromTextItem(const QString &text, const QPointF &pos, Scene *scene)
{
    AddCommand* addCommand = new AddCommand;
    addCommand->textItem_ = new TextItem;
    addCommand->textItem_->setPlainText(text);
    addCommand->initialPosition_ = pos  - addCommand->textItem_->boundingRect().center();
    addCommand->scene_ = scene;

    addCommand->textItem_->enableTextEditing();

    return addCommand;
}

void AddCommand::undo()
{
    qDebug() << "add undo";
    if (shapeItem_) {
        scene_->removeItem(shapeItem_);
    } else {
        scene_->removeItem(textItem_);
    }
}

void AddCommand::redo()
{
    qDebug() << "add redo";
    if (shapeItem_) {
        scene_->addItem(shapeItem_);
        shapeItem_->setPos(initialPosition_);
    } else {
        scene_->addItem(textItem_);
        textItem_->setPos(initialPosition_);
    }
}
