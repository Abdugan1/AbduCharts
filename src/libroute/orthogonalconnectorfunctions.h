#ifndef ORTHOGONALCONNECTORFUNCTIONS_H
#define ORTHOGONALCONNECTORFUNCTIONS_H

class QRectF;
class QGraphicsItem;
class QPointF;
class Grid;

enum class ConnectionSide;

template<typename T> class QList;

QRectF inflatedBoundingRect(const QGraphicsItem* item, int margin = 0);

QRectF getOverallBoundingRect(const QRectF& startItemBoundingRect,
                              const QRectF& endItemBoundingRect,
                              int margins = 20);

bool isVerticalSide(ConnectionSide side);

bool isHorizontalSide(ConnectionSide side);

Grid rulersToGrid(const QList<double> verticals,
                  const QList<double> horizontals,
                  const QRectF &boundingRect);

void deletePointsContainingInObstacles(QList<QPointF> *gridPoints,
                                       const QList<QRectF> &obstacles);

QPointF getConnectionPoint(const QRectF &itemBoundingRect, ConnectionSide side);

#endif // ORTHOGONALCONNECTORFUNCTIONS_H
