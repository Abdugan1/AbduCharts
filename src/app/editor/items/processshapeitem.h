#ifndef PROCESSSHAPEITEM_H
#define PROCESSSHAPEITEM_H

#include <editor/items/flowchartshapeitem.h>

class ProcessShapeItem : public FlowchartShapeItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 13 };

    explicit ProcessShapeItem(QGraphicsItem* parent = nullptr);

    int type() const override;
};

#endif // PROCESSSHAPEITEM_H
