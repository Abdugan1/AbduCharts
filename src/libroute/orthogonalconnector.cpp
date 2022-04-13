#include "orthogonalconnector.h"
#include "orthogonalconnectorauto.h"
#include "orthogonalconnectormanual.h"

#include "pointgraph.h"

QList<QLineF> OrthogonalConnector::route(const ShapeInfo &startShapeInfo, const ShapeInfo &endShapeInfo)
{
    return OrthogonalConnectorAuto::route(startShapeInfo, endShapeInfo);
}

QList<QLineF> OrthogonalConnector::route(const ShapeInfo &startShapeInfo,
                                         const ShapeInfo &endShapeInfo,
                                         const QList<Waypoint> &waypoints)
{
    return OrthogonalConnectorManual::route(startShapeInfo, endShapeInfo, waypoints);
}

PointGraph OrthogonalConnector::routePointGraph(const ShapeInfo &startShapeInfo,
                                                const ShapeInfo &endShapeInfo,
                                                const QList<Waypoint> &waypoints)
{
    if (waypoints.empty()) {
        return OrthogonalConnectorAuto::routePointGraph(startShapeInfo, endShapeInfo);
    } else {
        return OrthogonalConnectorManual::routePointGraph(startShapeInfo, endShapeInfo, waypoints);
    }
}
