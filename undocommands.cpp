#include "undocommands.h"
#include "flowchartitems.h"
#include "textitems.h"
#include "scene.h"

#include <QDebug>

AddCommand *AddCommand::fromShapeItem(const QString &shapeType, const QPointF &pos, Scene *scene)
{
    AddCommand* addCommand = new AddCommand;
    addCommand->shapeItem_ = ShapeCreator::createShape(shapeType);
    addCommand->initialPosition_ = pos;
    addCommand->scene_ = scene;
    return addCommand;
}

AddCommand *AddCommand::fromTextItem(const QString &text, const QPointF &pos, Scene *scene)
{
    AddCommand* addCommand = new AddCommand;
    addCommand->textItem_ = new TextItem;
    addCommand->textItem_->setPlainText(text);
    addCommand->initialPosition_ = pos  - addCommand->textItem_->boundingRect().center();
    addCommand->scene_ = scene;
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

FlowchartTextItem *AddCommand::textItem() const
{
    return textItem_;
}


MoveCommand::MoveCommand(QGraphicsItem* shapeItem, const QPointF& oldPos)
    : item_(shapeItem)
    , oldPos_(oldPos)
    , newPos_(shapeItem->pos())
{
}

void MoveCommand::undo()
{
    qDebug() << "move undo";
    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    item_->setPos(oldPos_);
    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void MoveCommand::redo()
{
    qDebug() << "move redo";
    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    item_->setPos(newPos_);
    item_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

ResizeCommand::ResizeCommand(FlowchartShapeItem *shapeItem, const QRectF &newRect, const QRectF &oldRect)
    : shapeItem_(shapeItem)
    , newRect_(newRect)
    , oldRect_(oldRect)
{
}

void ResizeCommand::undo()
{

}

void ResizeCommand::redo()
{

}
