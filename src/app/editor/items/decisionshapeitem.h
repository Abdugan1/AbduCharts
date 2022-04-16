#ifndef DECISIONSHAPEITEM_H
#define DECISIONSHAPEITEM_H

#include <editor/items/flowchartshapeitem.h>

class DecisionShapeItem : public FlowchartShapeItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 11 };

    explicit DecisionShapeItem(QGraphicsItem* parent = nullptr);

    int type() const override;
};

#endif // DECISIONSHAPEITEM_H
