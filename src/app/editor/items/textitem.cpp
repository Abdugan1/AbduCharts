#include "textitem.h"
#include "editor/grid.h"

#include <QDebug>

TextItem::TextItem(QGraphicsItem *parent)
    : FlowchartTextItem(parent)
{
    setPlainText("Im a TextItem!!!");
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

int TextItem::type() const
{
    return Type;
}

QVariant TextItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && Grid::enabled()) {
        return Grid::snapToGrid(value.toPointF());
    }
    if (change == ItemSelectedHasChanged && !value.toBool()) {
        qDebug() << "lost selection";
        disableTextEditing();
    }
    return FlowchartTextItem::itemChange(change, value);
}

void TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    FlowchartTextItem::mouseDoubleClickEvent(event);
    if (textInteractionFlags() == Qt::NoTextInteraction)
        enableTextEditing();
}
