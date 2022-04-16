#include "orthogonalconnector.h"
#include "orthogonalconnectorauto.h"
#include "orthogonalconnectormanual.h"

#include "pointgraph.h"

QList<QLineF> OrthogonalConnector::routeAutomatic(const ShapeInfo &startShapeInfo, const ShapeInfo &endShapeInfo)
{
    return OrthogonalConnectorAuto::route(startShapeInfo, endShapeInfo);
}

QList<QLineF> OrthogonalConnector::routeOnlyByWaypoints(const ShapeInfo &startShapeInfo,
                                                        const ShapeInfo &endShapeInfo,
                                                        const QList<Waypoint> &waypoints)
{
    return OrthogonalConnectorManual::route(startShapeInfo, endShapeInfo, waypoints);
}
