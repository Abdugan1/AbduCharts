#ifndef INOUTSHAPEITEM_H
#define INOUTSHAPEITEM_H

#include <editor/items/flowchartshapeitem.h>

class InOutShapeItem : public FlowchartShapeItem
{
    Q_OBJECT
public:
    explicit InOutShapeItem(QGraphicsItem* parent = nullptr);

    QString figureType() const override;
};

#endif // INOUTSHAPEITEM_H
