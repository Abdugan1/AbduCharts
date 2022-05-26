#ifndef DELETETEXTCOMMAND_H
#define DELETETEXTCOMMAND_H

#include "deletecommand.h"

class DeleteTextCommand : public DeleteCommand
{
public:
    explicit DeleteTextCommand(FlowchartTextItem *textItem, Scene *scene);

    void undo() override;
    void redo() override;

private:
    FlowchartTextItem* textItem_ = nullptr;
};

#endif // DELETETEXTCOMMAND_H
