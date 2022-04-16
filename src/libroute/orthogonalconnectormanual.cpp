#include "orthogonalconnectormanual.h"
#include "orthogonalconnectorfunctions.h"
#include "shapeinfo.h"
#include "grid.h"
#include "pointgraph.h"
#include "astar.h"
#include "node.h"

#include <QGraphicsItem>
#include <QPainterPath>
#include <QSet>
#include <QDebug>

inline uint qHash(const QPointF &p)
{
   return qHash(QPair<qreal, qreal> (p.x(), p.y()) );
}

const int DefaultShapeMargin    = 0;
const int DefaultRoutingMargin  = 20;
const int MinShapeMargin        = 0;
const int MaxShapeMargin        = 50;
const int MinRoutingMargin      = 1;
const int MaxRoutingMargin      = 100;

QMargins OrthogonalConnectorManual::shapeMargin_   = QMargins(DefaultShapeMargin, DefaultShapeMargin,
                                                              DefaultShapeMargin, DefaultShapeMargin);

QMargins OrthogonalConnectorManual::routingMargin_ = QMargins(DefaultRoutingMargin, DefaultRoutingMargin,
                                                              DefaultRoutingMargin, DefaultRoutingMargin);

QList<QLineF> simplifyLines(const QList<QLineF>& lines)
{
    auto isTurned = [](const QPointF& before, const QPointF& current, const QPointF& after) {
        bool turned = false;
        if (current.x() == before.x()) {
            turned = (after.x() != current.x());
        } else {
            turned = (after.y() != current.y());
        }
        return turned;
    };

    QList<QLineF> simplyfied;
    if (!lines.empty()) {
        auto it = lines.begin();
        QPointF before  = it->p1();
        QPointF current = it->p2();;
        QPointF after;

        while (++it != lines.end()) {
            after = it->p2();
            if (isTurned(before, current, after)) {
                if (before != current)
                    simplyfied.append({before, current});
                before = current;
            }
            current = after;
        }
        simplyfied.append({before, current});
    }
    return simplyfied;
}

QList<QLineF> OrthogonalConnectorManual::route(const ShapeInfo &startShapeInfo,
                                               const ShapeInfo &endShapeInfo,
                                               const QList<Waypoint> &waypoints)
{
    QRectF startItemBoundingRect = inflatedBoundingRect(startShapeInfo.item());
    QRectF endItemBoundingRect   = inflatedBoundingRect(endShapeInfo.item());

    QRectF boundingRect = calculateBoundingRect(startItemBoundingRect,
                                                endItemBoundingRect,
                                                waypoints);

    QList<double> verticals   = getVerticalRulersFromWaypoints(waypoints);
    QList<double> horizontals = getHorizontalRulersFromWaypoints(waypoints);

    // add origin points
    double startOrigin = (isVerticalSide(startShapeInfo.connectionSide()) ? startItemBoundingRect.center().x()
                                                              : startItemBoundingRect.center().y());

    double endOrigin = (isVerticalSide(endShapeInfo.connectionSide()) ? endItemBoundingRect.center().x()
                                                          : endItemBoundingRect.center().y());

    (isVerticalSide(startShapeInfo.connectionSide()) ? verticals : horizontals).push_back(startOrigin);
    (isVerticalSide(endShapeInfo.connectionSide())   ? verticals : horizontals).push_back(endOrigin);

    std::sort(verticals.begin(),   verticals.end());
    std::sort(horizontals.begin(), horizontals.end());

    Grid grid = rulersToGrid(verticals, horizontals, boundingRect);
    QList<QPointF> gridPoints = gridToSpots(grid, {startItemBoundingRect, endItemBoundingRect});

    QPointF startConnectionPoint = getConnectionPoint(startItemBoundingRect, startShapeInfo.connectionSide());
    QPointF endConnectionPoint   = getConnectionPoint(endItemBoundingRect,   endShapeInfo.connectionSide());

    gridPoints << startConnectionPoint << endConnectionPoint;
    for (const auto& waypoint : waypoints) {
        gridPoints << waypoint.pos();
    }

    PointGraph pointGraph = createPointGraph(gridPoints, {startItemBoundingRect, endItemBoundingRect});

    Node* startNode = pointGraph.get(startConnectionPoint);
    Node* endNode   = pointGraph.get(endConnectionPoint);

    QList<QLineF> path;
    for (const auto& waypoint : waypoints) {
        endNode = pointGraph.get(waypoint.pos());
        path << AStar::findPath(endNode, startNode);
        // if dont reset nodes, path wouln't find
        //            endNode->reset();
        //            startNode->reset();

        // if waypoints is not ordered, need to reset all nodes!
        auto map = pointGraph.nodes();
        for (auto i : map) {
            for (auto node : i) {
                node->reset();
            }
        }


        startNode = endNode;
    }
    endNode = pointGraph.get(endConnectionPoint);
    path << AStar::findPath(endNode, startNode);

    return simplifyLines(path);
}

QRectF OrthogonalConnectorManual::calculateBoundingRect(const QRectF &startItemBoundingRect,
                                                        const QRectF &endItemBoundingRect,
                                                        const QList<Waypoint> &waypoints)
{
    QPointF topLeft(qMin(startItemBoundingRect.left(), endItemBoundingRect.left()),
                    qMin(startItemBoundingRect.top(),  endItemBoundingRect.top()));

    QPointF bottomRight(qMax(startItemBoundingRect.right(),  endItemBoundingRect.right()),
                        qMax(startItemBoundingRect.bottom(), endItemBoundingRect.bottom()));

    QRectF boundingRect(topLeft, bottomRight);
    boundingRect += routingMargin_;

    if (!waypoints.empty()) {
        topLeft = boundingRect.topLeft();
        bottomRight = boundingRect.bottomRight();
        for (const auto& waypoint : waypoints) {
            topLeft = {qMin(topLeft.x(), waypoint.pos().x()),
                       qMin(topLeft.y(), waypoint.pos().y())};

            bottomRight = {qMax(bottomRight.x(), waypoint.pos().x()),
                           qMax(bottomRight.y(), waypoint.pos().y())};
        }

        if (boundingRect.topLeft() != topLeft || boundingRect.bottomRight() != bottomRight)
            boundingRect = QRectF(topLeft, bottomRight);
    }

    return boundingRect;
}

QList<double> OrthogonalConnectorManual::getVerticalRulersFromWaypoints(const QList<Waypoint> &waypoints)
{
    QList<double> verticals;
    for (const auto& waypoint : waypoints) {
        if (waypoint.moveOrientation() == MoveOrientation::LeftRight) {
            verticals.append(waypoint.pos().x());
        }
    }
    return verticals;
}

QList<double> OrthogonalConnectorManual::getHorizontalRulersFromWaypoints(const QList<Waypoint> &waypoints)
{
    QList<double> horizontals;
    for (const auto& waypoint : waypoints) {
        if (waypoint.moveOrientation() == MoveOrientation::UpDown) {
            horizontals.append(waypoint.pos().y());
        }
    }
    return horizontals;
}

QList<QPointF> OrthogonalConnectorManual::gridToSpots(const Grid &grid, const QList<QRectF> &obstacles)
{
    int rows = grid.rows();
    int cols = grid.cols();

    QSet<QPointF> allGridPoints;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {

            QRectF gridRect = grid.get(row, col);
            allGridPoints << gridRect.topLeft() << gridRect.topRight()
                          << gridRect.bottomLeft() << gridRect.bottomRight();
        }
    }

    QList<QPointF> gridPoints = allGridPoints.values();
    deletePointsContainingInObstacles(&gridPoints, obstacles);

    return gridPoints;
}

PointGraph OrthogonalConnectorManual::createPointGraph(const QList<QPointF> &gridPoints,
                                                       const QList<QRectF> &obstacleRects)
{
    PointGraph pointGraph;
    QVector<double> uniqueXs;
    QVector<double> uniqueYs;

    for (const auto& gridPoint : gridPoints) {
        if (!uniqueXs.contains(gridPoint.x()))
            uniqueXs.push_back(gridPoint.x());

        if (!uniqueYs.contains(gridPoint.y()))
            uniqueYs.push_back(gridPoint.y());

        pointGraph.add(gridPoint);
    }

    std::sort(uniqueXs.begin(), uniqueXs.end());
    std::sort(uniqueYs.begin(), uniqueYs.end());

    for (int i = 0; i < uniqueYs.size(); ++i) {
        for (int j = 0; j < uniqueXs.size(); ++j) {
            QPointF b(uniqueXs[j], uniqueYs[i]);

            if (!gridPoints.contains(b))
                continue;

            if (j > 0) {
                int k = j - 1;
                QPointF a(uniqueXs[j - 1], uniqueYs[i]);
                bool obstacleCollision = false;
                while (!gridPoints.contains(a) && k > 0 && !obstacleCollision) {
                    a = QPointF(uniqueXs[--k], uniqueYs[i]);
                    for (const auto& rect : obstacleRects) {
                        if (rect.contains(a)) {
                            obstacleCollision = true;
                            break;
                        }
                    }
                }

                if (gridPoints.contains(a) && !obstacleCollision) {
                    pointGraph.connect(a, b);
                    pointGraph.connect(b, a);
                }
            }

            if (i > 0) {
                int k = i - 1;
                QPointF a(uniqueXs[j], uniqueYs[i - 1]);
                bool obstacleCollision = false;
                while (!gridPoints.contains(a) && k > 0 && !obstacleCollision) {
                    a = QPointF(uniqueXs[j], uniqueYs[--k]);
                    for (const auto& rect : obstacleRects) {
                        if (rect.contains(a)) {
                            obstacleCollision = true;
                            break;
                        }
                    }
                }

                if (gridPoints.contains(a)) {
                    pointGraph.connect(a, b);
                    pointGraph.connect(b, a);
                }
            }
        }
    }

    return pointGraph;
}
