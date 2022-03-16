#ifndef PROCESSSHAPEITEM_H
#define PROCESSSHAPEITEM_H

#include <editor/items/flowchartshapeitem.h>

class ProcessShapeItem : public FlowchartShapeItem
{
    Q_OBJECT
public:
    explicit ProcessShapeItem(QGraphicsItem* parent = nullptr);

    QString figureType() const override;
};

#endif // PROCESSSHAPEITEM_H
