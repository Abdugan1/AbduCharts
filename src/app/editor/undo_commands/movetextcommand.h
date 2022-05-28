#ifndef MOVETEXTCOMMAND_H
#define MOVETEXTCOMMAND_H

#include "movecommand.h"

class MoveTextCommand : public MoveCommand
{
public:
    explicit MoveTextCommand(FlowchartTextItem* textItem, const QPointF& oldPos);

    void undo() override;
    void redo() override;

private:
    FlowchartTextItem* textItem_ = nullptr;
};

#endif // MOVETEXTCOMMAND_H
