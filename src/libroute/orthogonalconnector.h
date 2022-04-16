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
    static QList<QLineF> routeAutomatic(const ShapeInfo& startShapeInfo,
                                        const ShapeInfo& endShapeInfo);

    static QList<QLineF> routeOnlyByWaypoints(const ShapeInfo& startShapeInfo,
                                              const ShapeInfo& endShapeInfo,
                                              const QList<Waypoint>& waypoints);
};

#endif // ORTHOGONALCONNECTOR_H
