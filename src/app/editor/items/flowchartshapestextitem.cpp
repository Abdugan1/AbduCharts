#include "flowchartshapestextitem.h"
#include "flowchartshapeitem.h"

#include <QTextDocument>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

FlowchartShapesTextItem::FlowchartShapesTextItem(FlowchartShapeItem *parent)
    : FlowchartTextItem(parent)
    , myItem_(parent)
{
    connect(parent, &FlowchartShapeItem::resizedByHands, this, &FlowchartShapesTextItem::centerOnShapeItem);
    connect(document(), &QTextDocument::contentsChanged, this, &FlowchartShapesTextItem::centerOnShapeItem);

    centerOnShapeItem();
}

FlowchartShapesTextItem::~FlowchartShapesTextItem()
{
    qDebug() << "FlowchartShapesTextItem: Destructor";
}

int FlowchartShapesTextItem::type() const
{
    return Type;
}

void FlowchartShapesTextItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    FlowchartTextItem::paint(painter, option, widget);
}

void FlowchartShapesTextItem::enableTextEditingAndMousePress()
{
    acceptMousePress_ = true;
    enableTextEditing();
}

void FlowchartShapesTextItem::disableTextEditingAndMousePress()
{
    acceptMousePress_ = false;
    disableTextEditing();
}

void FlowchartShapesTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "FlowchartShapesTextItem: mousePress";
    if (!acceptMousePress_) {
        event->ignore();
    } else {
        FlowchartTextItem::mousePressEvent(event);
    }
}

void FlowchartShapesTextItem::centerOnShapeItem()
{
    setPos(-boundingRect().width() / 2, -boundingRect().height() / 2);
}
