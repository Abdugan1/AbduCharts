#include "orthogonalconnectorauto.h"
#include "orthogonalconnectorfunctions.h"
#include "shapeinfo.h"
#include "grid.h"
#include "pointgraph.h"
#include "astar.h"

#include <QGraphicsItem>
#include <QPainterPath>
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

QMargins OrthogonalConnectorAuto::shapeMargin_   = QMargins(DefaultShapeMargin, DefaultShapeMargin,
                                                            DefaultShapeMargin, DefaultShapeMargin);

QMargins OrthogonalConnectorAuto::routingMargin_ = QMargins(DefaultRoutingMargin, DefaultRoutingMargin,
                                                            DefaultRoutingMargin, DefaultRoutingMargin);

QList<QLineF> OrthogonalConnectorAuto::route(const ShapeInfo &startShapeInfo,
                                             const ShapeInfo &endShapeInfo)
{
    QRectF startItemBoundingRect = inflatedBoundingRect(startShapeInfo.item());
    QRectF endItemBoundingRect   = inflatedBoundingRect(endShapeInfo.item());

    QRectF boundingRect = getOverallBoundingRect(startItemBoundingRect,
                                                 endItemBoundingRect);

    QList<double> verticals   = getVerticalRulersFromItems(startItemBoundingRect,   endItemBoundingRect);
    QList<double> horizontals = getHorizontalRulersFromItems(startItemBoundingRect, endItemBoundingRect);

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

    PointGraph pointGraph = createPointGraph(gridPoints);

    Node* startNode = pointGraph.get(startConnectionPoint);
    Node* endNode   = pointGraph.get(endConnectionPoint);

    return AStar::findPath(endNode, startNode);
}

PointGraph OrthogonalConnectorAuto::routePointGraph(const ShapeInfo &startShapeInfo, const ShapeInfo &endShapeInfo)
{
    QRectF startItemBoundingRect = inflatedBoundingRect(startShapeInfo.item());
    QRectF endItemBoundingRect   = inflatedBoundingRect(endShapeInfo.item());

    QRectF boundingRect = getOverallBoundingRect(startItemBoundingRect,
                                                 endItemBoundingRect);

    QList<double> verticals   = getVerticalRulersFromItems(startItemBoundingRect,   endItemBoundingRect);
    QList<double> horizontals = getHorizontalRulersFromItems(startItemBoundingRect, endItemBoundingRect);

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

    PointGraph pointGraph = createPointGraph(gridPoints);

    Node* startNode = pointGraph.get(startConnectionPoint);
    Node* endNode   = pointGraph.get(endConnectionPoint);

    return pointGraph;
}

QList<double> OrthogonalConnectorAuto::getVerticalRulersFromItems(const QRectF &startItemBoundingRect,
                                                                  const QRectF &endItemBoundingRect)
{
    return {startItemBoundingRect.left(), startItemBoundingRect.right(),
                endItemBoundingRect.left(),   endItemBoundingRect.right()};
}

QList<double> OrthogonalConnectorAuto::getHorizontalRulersFromItems(const QRectF &startItemBoundingRect,
                                                                    const QRectF &endItemBoundingRect)
{
    return {startItemBoundingRect.top(), startItemBoundingRect.bottom(),
                endItemBoundingRect.top(),   endItemBoundingRect.bottom()};
}

QList<QPointF> OrthogonalConnectorAuto::gridToSpots(const Grid &grid, const QList<QRectF> &obstacles)
{
    int rows = grid.rows();
    int cols = grid.cols();

    QSet<QPointF> allGridPoints;

    for (int row = 0; row < rows; ++row) {
        bool firstRow = (row == 0);
        bool lastRow  = (row == rows - 1);
        for (int col = 0; col < cols; ++col) {
            bool firstCol = (col == 0);
            bool lastCol  = (col == cols - 1);

            bool cornerCell = ((firstCol && firstRow)  || (firstRow && lastCol)
                               || (lastRow && lastCol) || (lastRow && firstCol));

            QRectF gridRect = grid.get(row, col);
            if (cornerCell) {
                allGridPoints << gridRect.topLeft() << gridRect.topRight()
                              << gridRect.bottomLeft() << gridRect.bottomRight();
            } else if (firstRow) {
                allGridPoints << gridRect.topLeft() << QPointF(gridRect.center().x(), gridRect.top())
                              << gridRect.topRight();
            } else if (lastRow) {
                allGridPoints << gridRect.bottomRight() << QPointF(gridRect.center().x(), gridRect.bottom())
                              << gridRect.bottomLeft();
            } else if (firstCol) {
                allGridPoints << gridRect.topLeft() << QPointF(gridRect.left(), gridRect.center().y())
                              << gridRect.bottomLeft();
            } else if (lastCol) {
                allGridPoints << gridRect.topRight() << QPointF(gridRect.right(), gridRect.center().y())
                              << gridRect.bottomRight();
            } else {
                allGridPoints << gridRect.topLeft()     << QPointF(gridRect.center().x(), gridRect.top())
                              << gridRect.topRight()    << QPointF(gridRect.right(), gridRect.center().y())
                              << gridRect.bottomRight() << QPointF(gridRect.center().x(), gridRect.bottom())
                              << gridRect.bottomLeft()  << QPointF(gridRect.left(), gridRect.center().y())
                              << gridRect.center();
            }
        }
    }

    QList<QPointF> gridPoints = allGridPoints.values();
    deletePointsContainingInObstacles(&gridPoints, obstacles);

    return gridPoints;
}

PointGraph OrthogonalConnectorAuto::createPointGraph(const QList<QPointF> &gridPoints)
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
                QPointF a(uniqueXs[j - 1], uniqueYs[i]);

                if (gridPoints.contains(a)) {
                    pointGraph.connect(a, b);
                    pointGraph.connect(b, a);
                }
            }

            if (i > 0) {
                QPointF a(uniqueXs[j], uniqueYs[i - 1]);

                if (gridPoints.contains(a)) {
                    pointGraph.connect(a, b);
                    pointGraph.connect(b, a);
                }
            }
        }
    }

    return pointGraph;
}
