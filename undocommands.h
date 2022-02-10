#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QUndoCommand>
#include <QPointF>
#include <QRectF>

class FlowchartShapeItem;
class FlowchartTextItem;
class Scene;

class QGraphicsItem;

class AddCommand : public QUndoCommand
{
public:
    static AddCommand* fromShapeItem(const QString& shapeType, const QPointF& pos, Scene* scene);
    static AddCommand* fromTextItem(const QString& text, const QPointF& pos, Scene* scene);

    void undo() override;
    void redo() override;

    FlowchartTextItem *textItem() const;

private:
    AddCommand() {};

private:
    FlowchartShapeItem* shapeItem_ = nullptr;
    FlowchartTextItem*  textItem_  = nullptr;

    QPointF initialPosition_;

    Scene* scene_ = nullptr;
};


//------------------------------------------------
//                MoveCommand
//------------------------------------------------


class MoveCommand : public QUndoCommand
{
public:
    explicit MoveCommand(QGraphicsItem* shapeItem, const QPointF& oldPos);

    void undo() override;
    void redo() override;

private:
    QGraphicsItem* item_ = nullptr;

    QPointF oldPos_;
    QPointF newPos_;
};


//------------------------------------------------
//                ResizeCommand
//------------------------------------------------


class ResizeCommand : public QUndoCommand
{
public:
    explicit ResizeCommand(FlowchartShapeItem* shapeItem, const QRectF& newRect, const QRectF& oldRect);

    void undo() override;
    void redo() override;

private:
    FlowchartShapeItem* shapeItem_ = nullptr;
    QRectF newRect_;
    QRectF oldRect_;
};


#endif // UNDOCOMMANDS_H
