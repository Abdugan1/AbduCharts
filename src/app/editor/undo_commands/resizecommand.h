#ifndef RESIZECOMMAND_H
#define RESIZECOMMAND_H

#include <QUndoCommand>
#include <QRectF>

class FlowchartShapeItem;

class ResizeCommand : public QUndoCommand
{
public:
    explicit ResizeCommand(FlowchartShapeItem* shapeItem,
                           const QRectF& oldRect,
                           const QRectF& newRect,
                           const QPointF& posBeforeResize,
                           const QPointF& posAfterResize);

    void undo() override;
    void redo() override;

private:
    FlowchartShapeItem* shapeItem_ = nullptr;
    QRectF oldRect_;
    QRectF newRect_;
    QPointF posBeforeResize_;
    QPointF posAfterResize_;
};

#endif // RESIZECOMMAND_H
