#ifndef TERMINALSHAPEITEM_H
#define TERMINALSHAPEITEM_H

#include <editor/items/flowchartshapeitem.h>

class TerminalShapeItem : public FlowchartShapeItem
{
    Q_OBJECT
public:
    explicit TerminalShapeItem(QGraphicsItem* parent = nullptr);

    QString figureType() const override;
};

#endif // TERMINALSHAPEITEM_H
