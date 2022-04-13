#ifndef ORTHOGONALCONNECTORAUTO_H
#define ORTHOGONALCONNECTORAUTO_H

#include <QList>

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

class OrthogonalConnectorAuto
{
public:
    static QList<QLineF> route(const ShapeInfo& startShapeInfo,
                               const ShapeInfo& endShapeInfo);

    static PointGraph routePointGraph(const ShapeInfo& startShapeInfo,
                                      const ShapeInfo& endShapeInfo);

private:
    static QList<double> getVerticalRulersFromItems(const QRectF& startItemBoundingRect,
                                                    const QRectF& endItemBoundingRect);

    static QList<double> getHorizontalRulersFromItems(const QRectF& startItemBoundingRect,
                                                      const QRectF& endItemBoundingRect);

    static QList<QPointF> gridToSpots(const Grid& grid, const QList<QRectF>& obstacles);

    static PointGraph createPointGraph(const QList<QPointF>& gridPoints);

private:
    static QMargins shapeMargin_;
    static QMargins routingMargin_;
};
#endif // ORTHOGONALCONNECTORAUTO_H
