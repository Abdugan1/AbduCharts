#ifndef ADDSHAPECOMMAND_H
#define ADDSHAPECOMMAND_H

#include "addcommand.h"

class FlowchartShapeItem;

class AddShapeCommand : public AddCommand
{
public:
    explicit AddShapeCommand(int itemType, const QPointF& pos, Scene* scene);

    void undo() override;
    void redo() override;

private:
    FlowchartShapeItem* shapeItem_ = nullptr;
};

#endif // ADDSHAPECOMMAND_H
