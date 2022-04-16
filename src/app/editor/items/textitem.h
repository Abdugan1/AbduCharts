#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "flowcharttextitem.h"

class TextItem : public FlowchartTextItem
{
    Q_OBJECT
public:
    enum {Type = UserType + 1002};

    explicit TextItem(QGraphicsItem* parent = nullptr);

    int type() const override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};
#endif // TEXTITEM_H
