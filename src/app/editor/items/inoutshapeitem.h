#ifndef INOUTSHAPEITEM_H
#define INOUTSHAPEITEM_H

#include <editor/items/flowchartshapeitem.h>

class InOutShapeItem : public FlowchartShapeItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 12 };

    explicit InOutShapeItem(QGraphicsItem* parent = nullptr);

    int type() const override;
};

#endif // INOUTSHAPEITEM_H
