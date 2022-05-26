#ifndef DELETESHAPECOMMAND_H
#define DELETESHAPECOMMAND_H

#include "deletecommand.h"

class DeleteShapeCommand : public DeleteCommand
{
public:
    explicit DeleteShapeCommand(FlowchartShapeItem* shapeItem, Scene* scene);

    void undo() override;
    void redo() override;

private:
    FlowchartShapeItem* shapeItem_ = nullptr;
};

#endif // DELETESHAPECOMMAND_H
