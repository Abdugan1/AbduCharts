#ifndef ORTHOGONALCONNECTORMANUAL_H
#define ORTHOGONALCONNECTORMANUAL_H

#include <QList>
#include "waypoint.h"

class QPainterPath;
class QRectF;
class QPointF;
class QLineF;
class QGraphicsItem;
class QMargins;
class ShapeInfo;
class Grid;
class PointGraph;

enum class ConnectionSide;

class OrthogonalConnectorManual
{
public:
    static QList<QLineF> route(const ShapeInfo& startShapeInfo,
                               const ShapeInfo& endShapeInfo,
                               const QList<Waypoint>& waypoints);

    static PointGraph routePointGraph(const ShapeInfo& startShapeInfo,
                                      const ShapeInfo& endShapeInfo,
                                      const QList<Waypoint>& waypoints = QList<Waypoint>());

private:
    static QRectF calculateBoundingRect(const QRectF& startItemBoundingRect,
                                        const QRectF& endItemBoundingRect,
                                        const QList<Waypoint>& waypoints);

    static QList<double> getVerticalRulersFromWaypoints(const QList<Waypoint>& waypoints);

    static QList<double> getHorizontalRulersFromWaypoints(const QList<Waypoint>& waypoints);

    static QList<QPointF> gridToSpots(const Grid& grid, const QList<QRectF>& obstacles);

    static PointGraph createPointGraph(const QList<QPointF>& gridPoints,
                                       const QList<QRectF>& obstacleRects);
private:
    static QMargins shapeMargin_;
    static QMargins routingMargin_;
};
#endif // ORTHOGONALCONNECTORMANUAL_H
