#ifndef ORTHOGONALCONNECTOR_H
#define ORTHOGONALCONNECTOR_H

#include <QList>
#include <QLineF>
#include "waypoint.h"

class ShapeInfo;
class PointGraph;

class OrthogonalConnector
{
public:
    static QList<QLineF> route(const ShapeInfo& startShapeInfo,
                               const ShapeInfo& endShapeInfo);

    static QList<QLineF> route(const ShapeInfo& startShapeInfo,
                               const ShapeInfo& endShapeInfo,
                               const QList<Waypoint>& waypoints);


    static PointGraph routePointGraph(const ShapeInfo& startShapeInfo,
                                      const ShapeInfo& endShapeInfo,
                                      const QList<Waypoint>& waypoints = QList<Waypoint>());
};

#endif // ORTHOGONALCONNECTOR_H
