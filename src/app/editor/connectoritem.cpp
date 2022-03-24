#include "connectoritem.h"

#include "libroute/orthogonalconnector.h"

#include <QPainter>
#include <QDebug>

ConnectorItem::ConnectorItem(QGraphicsItem *parent)
    : ShapeItemBase(parent)
{

}

ConnectorItem::~ConnectorItem()
{
    qDebug() << "ConnectorItem: Destructor";
}

QRectF ConnectorItem::boundingRect() const
{
    return contentRect() + margins();
}

QRectF ConnectorItem::contentRect() const
{
    return shape().boundingRect();
}

QPainterPath ConnectorItem::shape() const
{
    return linesToPath();
}

void ConnectorItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    for (const auto& line : qAsConst(connectionPath_))
        painter->drawLine(line);
}

void ConnectorItem::setEndShapeInfo(const ShapeInfo &endShapeInfo)
{
    endShapeInfo_ = endShapeInfo;
}

QGraphicsItem *ConnectorItem::startItem() const
{
    return startShapeInfo_.item;
}

QGraphicsItem *ConnectorItem::endItem() const
{
    return endShapeInfo_.item;
}

void ConnectorItem::updateConnectionPath()
{
    prepareGeometryChange();
    connectionPath_ = OrthogonalConnector::route(startShapeInfo_, endShapeInfo_);
}

QPainterPath ConnectorItem::linesToPath() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    const int margin = 5;
    for (const auto& line : qAsConst(connectionPath_)) {
        QPointF topLeft(qMin(line.p1().x(), line.p2().x()) - margin,
                        qMin(line.p1().y(), line.p2().y()) - margin);
        QPointF bottomRight(qMax(line.p1().x(), line.p2().x()) + margin,
                            qMax(line.p1().y(), line.p2().y()) + margin);
        QRectF rect(topLeft, bottomRight);
        path.addRect(rect);
    }
    return path;
}

void ConnectorItem::setStartShapeInfo(const ShapeInfo &startShapeInfo)
{
    startShapeInfo_ = startShapeInfo;
}
