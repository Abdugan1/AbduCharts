#include "editor/connectoritem.h"
#include "editor/connectorhandle.h"

#include "libroute/orthogonalconnector.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>


QPointF getConnectionPoint(const QRectF &itemBoundingRect, ConnectionSide side)
{
    switch (side) {
    case ConnectionSide::Top:    return QPointF(itemBoundingRect.center().x(), itemBoundingRect.top());
    case ConnectionSide::Left:   return QPointF(itemBoundingRect.left(), itemBoundingRect.center().y());
    case ConnectionSide::Bottom: return QPointF(itemBoundingRect.center().x(), itemBoundingRect.bottom());
    case ConnectionSide::Right:  return QPointF(itemBoundingRect.right(), itemBoundingRect.center().y());
    }
    Q_UNREACHABLE();
}


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

    drawArrowHead(painter);
}

void ConnectorItem::setEndShapeInfo(const ShapeInfo &endShapeInfo)
{
    endShapeInfo_ = endShapeInfo;
}

QGraphicsItem *ConnectorItem::startItem() const
{
    return startShapeInfo_.item();
}

QGraphicsItem *ConnectorItem::endItem() const
{
    return endShapeInfo_.item();
}

void ConnectorItem::updateConnectionPath()
{
    prepareGeometryChange();

    const int previousConnectionPathLineCount = connectionPath_.count();

    if (autoPathFinding_) {
        connectionPath_ = OrthogonalConnector::route(startShapeInfo_, endShapeInfo_);

    } else {
        QList<Waypoint> waypoints;
        for (const auto handle : qAsConst(connectorHandles_))
            waypoints.append(Waypoint(handle->pos(), handle->moveOrientation()));

        connectionPath_ = OrthogonalConnector::route(startShapeInfo_, endShapeInfo_, waypoints);
    }

    if (previousConnectionPathLineCount == connectionPath_.count()) {
        qDebug() << "Updating position of connector handles";
        updateConnectorHandlesPosition();
    } else {
        qDebug() << "Creating new connector handles";
        deleteConnectorHandles();
        generateConnectorHandles();
    }
}

void ConnectorItem::onConnectorHandleMoved(ConnectorHandle *handle, const QPointF &pos)
{
    autoPathFinding_ = false;

    prepareGeometryChange();

    updateConnectionPathBuMovedHandle(handle, pos);

    if ((handle->lineIndex() + 1 == 1) || (handle->lineIndex() + 1 == connectionPath_.count())) {
        ConnectorHandle* connectorHandle;
        if (handle->lineIndex() + 1 == 1) {
            QRectF itemRect = startItem()->boundingRect();
            itemRect.moveTopLeft(itemRect.topLeft() + startItem()->pos());

            QPointF p1(getConnectionPoint(itemRect, startShapeInfo_.connectionSide()));
            QPointF p2(connectionPath_.first().p1());
            connectionPath_.prepend({p1, p2});

            connectorHandle = createConnectorHandle(connectionPath_.first(), 0);
            connectorHandles_.prepend(connectorHandle);

            int i = 1;
            for (auto it = connectorHandles_.begin() + 1; it != connectorHandles_.end(); ++it) {
                (*it)->setLineIndex(i++);
            }

        } else if (handle->lineIndex() + 1 == connectionPath_.count()) {
            QRectF itemRect = endItem()->boundingRect();
            itemRect.moveTopLeft(itemRect.topLeft() + endItem()->pos());

            QPointF p1(connectionPath_.last().p2());
            QPointF p2(getConnectionPoint(itemRect, endShapeInfo_.connectionSide()));
            connectionPath_.append({p1, p2});

            connectorHandle = createConnectorHandle(connectionPath_.last(), connectionPath_.count() - 1);
            connectorHandles_.append(connectorHandle);

            int i = 0;
            for (auto it = connectorHandles_.begin(); it != (connectorHandles_.end() - 1); ++it) {
                (*it)->setLineIndex(i++);
            }
        }
    }
    updateConnectorHandlesPosition();
}

void ConnectorItem::deleteConnectorHandles()
{
    for (auto handle : connectorHandles_) {
        scene()->removeItem(handle);
        delete handle;
    }
    connectorHandles_.clear();
}

void ConnectorItem::generateConnectorHandles()
{
    int i = 0;
    for (const auto& line : qAsConst(connectionPath_)) {
        auto connectorHandle = createConnectorHandle(line, i++);
        connectorHandle->setPos(line.center());
        connectorHandles_.append(connectorHandle);
    }
}

void ConnectorItem::updateConnectorHandlesPosition()
{
    auto handlesIterator = connectorHandles_.begin();
    for (auto& line : connectionPath_) {
        (*handlesIterator)->setPos(line.center());
        ++handlesIterator;
    }
}

void ConnectorItem::updateConnectionPathBuMovedHandle(const ConnectorHandle *handle, const QPointF &movedPos)
{
    int currentLineIndex = handle->lineIndex();
    QLineF currentLine = connectionPath_.at(currentLineIndex);
    if (handle->moveOrientation() == MoveOrientation::UpDown) {
        currentLine.setP1({currentLine.p1().x(), movedPos.y()});
        currentLine.setP2({currentLine.p2().x(), movedPos.y()});
    } else {
        currentLine.setP1({movedPos.x(), currentLine.p1().y()});
        currentLine.setP2({movedPos.x(), currentLine.p2().y()});
    }
    connectionPath_.replace(currentLineIndex, currentLine);

    if (currentLineIndex > 0) {
        QLineF beforeLine = connectionPath_.at(currentLineIndex - 1);
        beforeLine.setP2(currentLine.p1());
        connectionPath_.replace(currentLineIndex - 1, beforeLine);
    }

    if (currentLineIndex < connectionPath_.count() - 1) {
        QLineF afterLine = connectionPath_.at(currentLineIndex + 1);
        afterLine.setP1(currentLine.p2());
        connectionPath_.replace(currentLineIndex + 1, afterLine);
    }
}

ConnectorHandle *ConnectorItem::createConnectorHandle(const QLineF &line, int lineIndex)
{
    auto connectorHandle = new ConnectorHandle(line, lineIndex, this);
    connect(connectorHandle, &ConnectorHandle::moved,
            this,            &ConnectorItem::onConnectorHandleMoved);
    return connectorHandle;

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

QPolygonF ConnectorItem::calculateArrow() const
{
    // routing begins from the end, so p1() is endPoint
    const qreal w = 5.0;
    const qreal h = 7.0;
    QPointF endPoint = connectionPath_.last().p2();
    QPolygonF arrow;

    switch (endShapeInfo_.connectionSide()) {
    case ConnectionSide::Top:
        arrow << QPointF(endPoint.x() - w / 2, endPoint.y() - h)
              << QPointF(endPoint.x() + w / 2, endPoint.y() - h)
              << endPoint;
        break;
    case ConnectionSide::Left:
        arrow << QPointF(endPoint.x() - h, endPoint.y() + w / 2)
              << QPointF(endPoint.x() - h, endPoint.y() - w / 2)
              << endPoint;
        break;
    case ConnectionSide::Right:
        arrow << QPointF(endPoint.x() + h, endPoint.y() - w / 2)
              << QPointF(endPoint.x() + h, endPoint.y() + w / 2)
              << endPoint;
        break;
    case ConnectionSide::Bottom:
        arrow << QPointF(endPoint.x() + w / 2, endPoint.y() + h)
              << QPointF(endPoint.x() - w / 2, endPoint.y() + h)
              << endPoint;
        break;
    }
    return arrow;
}

void ConnectorItem::drawArrowHead(QPainter *painter) const
{
    QPolygonF arrow = calculateArrow();
    painter->save();
    painter->setBrush(QBrush(Qt::black));
    painter->drawPolygon(arrow);
    painter->restore();
}

void ConnectorItem::setStartShapeInfo(const ShapeInfo &startShapeInfo)
{
    startShapeInfo_ = startShapeInfo;
}
