#ifndef FLOWCHARTSHAPESTEXTITEM_H
#define FLOWCHARTSHAPESTEXTITEM_H

#include "flowcharttextitem.h"

class FlowchartShapeItem;

class FlowchartShapesTextItem : public FlowchartTextItem
{
    Q_OBJECT
public:
    enum {Type = UserType + 1001};

    explicit FlowchartShapesTextItem(FlowchartShapeItem* parent = nullptr);
    ~FlowchartShapesTextItem();

    int type() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

public slots:
    void enableTextEditingAndMousePress();
    void disableTextEditingAndMousePress();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void centerOnShapeItem();

private:
    FlowchartShapeItem* myItem_ = nullptr;
    bool acceptMousePress_ = false;
};

#endif // FLOWCHARTSHAPESTEXTITEM_H
