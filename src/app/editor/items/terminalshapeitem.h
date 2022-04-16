#ifndef TERMINALSHAPEITEM_H
#define TERMINALSHAPEITEM_H

#include <editor/items/flowchartshapeitem.h>

class TerminalShapeItem : public FlowchartShapeItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 14 };

    explicit TerminalShapeItem(QGraphicsItem* parent = nullptr);

    int type() const override;
};

#endif // TERMINALSHAPEITEM_H
