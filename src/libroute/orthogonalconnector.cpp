#include "orthogonalconnector.h"
#include "shapeinfo.h"
#include "grid.h"
#include "pointgraph.h"
#include "astar.h"

#include <QGraphicsItem>
#include <QPainterPath>
#include <QSet>
#include <QPointF>
#include <QDebug>

uint qHash(const QPointF &p)
{
   return qHash(QPair<qreal, qreal> (p.x(), p.y()) );
}

const int DefaultShapeMargin    = 0;
const int DefaultRoutingMargin  = 20;
const int MinShapeMargin        = 0;
const int MaxShapeMargin        = 50;
const int MinRoutingMargin      = 1;
const int MaxRoutingMargin      = 100;

QMargins OrthogonalConnector::shapeMargin_   = QMargins(DefaultShapeMargin, DefaultShapeMargin,
                                                        DefaultShapeMargin, DefaultShapeMargin);

QMargins OrthogonalConnector::routingMargin_ = QMargins(DefaultRoutingMargin, DefaultRoutingMargin,
                                                        DefaultRoutingMargin, DefaultRoutingMargin);

QList<QLineF> OrthogonalConnector::route(const ShapeInfo &startShapeInfo, const ShapeInfo &endShapeInfo)
{
    QRectF startItemBoundingRect = calculateItemBoundingRect(startShapeInfo.item);
    QRectF endItemBoundingRect   = calculateItemBoundingRect(endShapeInfo.item);

    QRectF boundingRect = calculateBoundingRect(startItemBoundingRect, endItemBoundingRect);

    QList<double> verticals   = getVerticalRulers(startItemBoundingRect,   endItemBoundingRect);
    QList<double> horizontals = getHorizontalRulers(startItemBoundingRect, endItemBoundingRect);

    // add origin points
    double startOrigin = (isVerticalSide(startShapeInfo.side) ? startItemBoundingRect.center().x()
                                                              : startItemBoundingRect.center().y());

    double endOrigin = (isVerticalSide(endShapeInfo.side) ? endItemBoundingRect.center().x()
                                                          : endItemBoundingRect.center().y());

    (isVerticalSide(startShapeInfo.side) ? verticals : horizontals).push_back(startOrigin);
    (isVerticalSide(endShapeInfo.side)   ? verticals : horizontals).push_back(endOrigin);

    std::sort(verticals.begin(),   verticals.end());
    std::sort(horizontals.begin(), horizontals.end());

    Grid grid = rulersToGrid(verticals, horizontals, boundingRect);
    QList<QPointF> gridPoints = gridToSpots(grid, {startItemBoundingRect, endItemBoundingRect});

    QPointF startConnectionPoint = getConnectionPoint(startItemBoundingRect, startShapeInfo.side);
    QPointF endConnectionPoint   = getConnectionPoint(endItemBoundingRect,   endShapeInfo.side);

    gridPoints << startConnectionPoint << endConnectionPoint;

    PointGraph pointGraph = createPointGraph(gridPoints);

    Node* startNode = pointGraph.get(startConnectionPoint);
    Node* endNode   = pointGraph.get(endConnectionPoint);

    return AStar::findPath(startNode, endNode);
}

QRectF OrthogonalConnector::calculateItemBoundingRect(const QGraphicsItem *item)
{
    QRectF rect = item->boundingRect();
    rect.moveTopLeft(rect.topLeft() + item->pos());
    rect += shapeMargin_;
    return rect;
}

QRectF OrthogonalConnector::calculateBoundingRect(const QRectF &startItemBoundingRect,
                                                  const QRectF &endItemBoundingRect)
{
    QPointF topLeft(qMin(startItemBoundingRect.left(), endItemBoundingRect.left()),
                    qMin(startItemBoundingRect.top(),  endItemBoundingRect.top()));

    QPointF bottomRight(qMax(startItemBoundingRect.right(),  endItemBoundingRect.right()),
                        qMax(startItemBoundingRect.bottom(), endItemBoundingRect.bottom()));

    QRectF boundingRect(topLeft, bottomRight);
    boundingRect += routingMargin_;

    return boundingRect;
}

QList<double> OrthogonalConnector::getVerticalRulers(const QRectF &startItemBoundingRect,
                                                     const QRectF &endItemBoundingRect)
{
    QList<double> verticals{startItemBoundingRect.left(), startItemBoundingRect.right(),
                            endItemBoundingRect.left(),   endItemBoundingRect.right()};
    return verticals;
}

QList<double> OrthogonalConnector::getHorizontalRulers(const QRectF &startItemBoundingRect,
                                                       const QRectF &endItemBoundingRect)
{
    QList<double> horizontals{startItemBoundingRect.top(), startItemBoundingRect.bottom(),
                              endItemBoundingRect.top(),   endItemBoundingRect.bottom()};
    return horizontals;
}

bool OrthogonalConnector::isVerticalSide(ConnectionSide side)
{
    return (side == ConnectionSide::Top || side == ConnectionSide::Bottom);
}

bool OrthogonalConnector::isHorizontalSide(ConnectionSide side)
{
    return (side == ConnectionSide::Left || side == ConnectionSide::Right);
}

Grid OrthogonalConnector::rulersToGrid(const QList<double> verticals,
                                       const QList<double> horizontals,
                                       const QRectF &boundingRect)
{
    Grid grid;

    double lastX = boundingRect.left();
    double lastY = boundingRect.top();
    int row = 0;
    int col = 0;

    for (double y : horizontals) {
        for (double x : verticals) {
            grid.set(row, col++, QRectF(QPointF{lastX, lastY}, QPointF{x, y}));
            lastX = x;
        }

        // Last cell of the row
        grid.set(row, col, QRectF(QPointF{lastX, lastY}, QPointF{boundingRect.right(), y}));
        lastX = boundingRect.left();
        lastY = y;
        col = 0;
        ++row;
    }

    lastX = boundingRect.left();

    // Last row of cells
    for (double x : verticals) {
        grid.set(row, col++, QRectF(QPointF{lastX, lastY}, QPointF{x, boundingRect.bottom()}));
        lastX = x;
    }

    // Last cell of last row
    grid.set(row, col, QRectF(QPointF{lastX, lastY}, QPointF{boundingRect.right(), boundingRect.bottom()}));

    return grid;
}

QList<QPointF> OrthogonalConnector::gridToSpots(const Grid &grid, const QList<QRectF> &obstacles)
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

void OrthogonalConnector::deletePointsContainingInObstacles(QList<QPointF> *gridPoints,
                                                            const QList<QRectF> &obstacles)
{
    QList<QPointF> temp = *gridPoints;
    for (const auto& gridPoint : temp) {
        for (const auto& obstacle : obstacles) {
            if (obstacle.contains(gridPoint)) {
                gridPoints->removeAll(gridPoint);
            }
        }
    }
}

PointGraph OrthogonalConnector::createPointGraph(const QList<QPointF> &gridPoints)
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

QPointF OrthogonalConnector::getConnectionPoint(const QRectF &itemBoundingRect, ConnectionSide side)
{
    switch (side) {
    case ConnectionSide::Top:    return QPointF(itemBoundingRect.center().x(), itemBoundingRect.top());
    case ConnectionSide::Left:   return QPointF(itemBoundingRect.left(), itemBoundingRect.center().y());
    case ConnectionSide::Bottom: return QPointF(itemBoundingRect.center().x(), itemBoundingRect.bottom());
    case ConnectionSide::Right:  return QPointF(itemBoundingRect.right(), itemBoundingRect.center().y());
    }
    Q_UNREACHABLE();
}
