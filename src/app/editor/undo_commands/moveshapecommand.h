#ifndef MOVESHAPECOMMAND_H
#define MOVESHAPECOMMAND_H

#include "movecommand.h"

class MoveShapeCommand : public MoveCommand
{
public:
    explicit MoveShapeCommand(FlowchartShapeItem* shapeItem, const QPointF& oldPos);

    void undo() override;
    void redo() override;

private:
    FlowchartShapeItem* shapeItem_ = nullptr;
};

#endif // MOVESHAPECOMMAND_H
