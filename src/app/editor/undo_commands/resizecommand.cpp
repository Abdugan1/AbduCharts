#include "resizecommand.h"
#include "editor/items/flowchartshapeitem.h"

#include <QDebug>

ResizeCommand::ResizeCommand(FlowchartShapeItem *shapeItem,
                             const QRectF &oldRect,
                             const QRectF &newRect,
                             const QPointF &posBeforeResize,
                             const QPointF &posAfterResize)
    : shapeItem_(shapeItem)
    , oldRect_(oldRect)
    , newRect_(newRect)
    , posBeforeResize_(posBeforeResize)
    , posAfterResize_(posAfterResize)
{

}

void ResizeCommand::undo()
{
    qDebug() << "ResizeCommand: undo";
    shapeItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    shapeItem_->resize(oldRect_);
    shapeItem_->setPos(posBeforeResize_);
    shapeItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void ResizeCommand::redo()
{
    qDebug() << "ResizeCommand: redo";
    shapeItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    shapeItem_->resize(newRect_);
    shapeItem_->setPos(posAfterResize_);
    shapeItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
