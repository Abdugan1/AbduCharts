#ifndef ORTHOGONALCONNECTOR_H
#define ORTHOGONALCONNECTOR_H

class QPainterPath;
class QRectF;
class QPointF;
class QLineF;
class QGraphicsItem;
class QMargins;
class ShapeInfo;
class Grid;
class PointGraph;
template<typename T> class QList;

enum class ConnectionSide;

class OrthogonalConnector
{
public:
    static QList<QLineF> route(const ShapeInfo& startShapeInfo, const ShapeInfo& endShapeInfo);

private:
    static QRectF calculateItemBoundingRect(const QGraphicsItem* item);

    static QRectF calculateBoundingRect(const QRectF& startItemBoundingRect,
                                        const QRectF& endItemBoundingRect);

    static QList<double> getVerticalRulers(const QRectF& startItemBoundingRect,
                                           const QRectF& endItemBoundingRect);

    static QList<double> getHorizontalRulers(const QRectF& startItemBoundingRect,
                                             const QRectF& endItemBoundingRect);

    static bool isVerticalSide(ConnectionSide side);
    static bool isHorizontalSide(ConnectionSide side);

    static Grid rulersToGrid(const QList<double> verticals,
                             const QList<double> horizontals,
                             const QRectF& boundingRect);

    static QList<QPointF> gridToSpots(const Grid& grid, const QList<QRectF>& obstacles);

    static void deletePointsContainingInObstacles(QList<QPointF>* gridPoints, const QList<QRectF>& obstacles);

    static PointGraph createPointGraph(const QList<QPointF>& gridPoints);

    static QPointF getConnectionPoint(const QRectF& itemBoundingRect, ConnectionSide side);

private:
    static QMargins shapeMargin_;
    static QMargins routingMargin_;
};

#endif // ORTHOGONALCONNECTOR_H
