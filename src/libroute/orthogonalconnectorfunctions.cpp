#include "orthogonalconnectorfunctions.h"
#include "shapeinfo.h"
#include "grid.h"

#include <QGraphicsItem>

QRectF inflatedBoundingRect(const QGraphicsItem *item, int margin)
{
    QRectF rect = item->boundingRect();
    rect.moveTopLeft(rect.topLeft() + item->pos());
    QMarginsF margins(margin, margin, margin, margin);
    rect += margins;
    return rect;
}

QRectF getOverallBoundingRect(const QRectF &startItemBoundingRect, const QRectF &endItemBoundingRect, int margin)
{
    QPointF topLeft(qMin(startItemBoundingRect.left(), endItemBoundingRect.left()),
                    qMin(startItemBoundingRect.top(),  endItemBoundingRect.top()));

    QPointF bottomRight(qMax(startItemBoundingRect.right(),  endItemBoundingRect.right()),
                        qMax(startItemBoundingRect.bottom(), endItemBoundingRect.bottom()));

    QRectF boundingRect(topLeft, bottomRight);
    QMarginsF margins(margin, margin, margin, margin);
    boundingRect += margins;

    return boundingRect;
}

bool isVerticalSide(ConnectionSide side)
{
    return (side == ConnectionSide::Top || side == ConnectionSide::Bottom);
}

bool isHorizontalSide(ConnectionSide side)
{
    return (side == ConnectionSide::Left || side == ConnectionSide::Right);
}

Grid rulersToGrid(const QList<double> verticals,
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

void deletePointsContainingInObstacles(QList<QPointF> *gridPoints,
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

QPointF getConnectionPoint(const QRectF &itemBoundingRect, ConnectionSide side)
{
    switch (side) {
    case ConnectionSide::Top:    return QPointF(itemBoundingRect.center().x(), itemBoundingRect.top());
    case ConnectionSide::Left:   return QPointF(itemBoundingRect.left(), itemBoundingRect.center().y());
    case ConnectionSide::Bottom: return QPointF(itemBoundingRect.center().x(), itemBoundingRect.bottom());
    case ConnectionSide::Right:  return QPointF(itemBoundingRect.right(), itemBoundingRect.center().y());
    }
    Q_UNREACHABLE();
}
