#ifndef DECISIONSHAPEITEM_H
#define DECISIONSHAPEITEM_H

#include <editor/items/flowchartshapeitem.h>

class DecisionShapeItem : public FlowchartShapeItem
{
    Q_OBJECT
public:
    explicit DecisionShapeItem(QGraphicsItem* parent = nullptr);

    QString figureType() const override;
};

#endif // DECISIONSHAPEITEM_H
