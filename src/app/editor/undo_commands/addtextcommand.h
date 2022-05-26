#ifndef ADDTEXTCOMMAND_H
#define ADDTEXTCOMMAND_H

#include "addcommand.h"

class FlowchartTextItem;

class AddTextCommand : public AddCommand
{
public:
    explicit AddTextCommand(const QString& text, const QPointF& pos, Scene* scene);

    void undo() override;
    void redo() override;

private:
    FlowchartTextItem* textItem_ = nullptr;
};

#endif // ADDTEXTCOMMAND_H
