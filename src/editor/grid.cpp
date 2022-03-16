#include "editor/grid.h"

int Grid::gridSize_ = 20;
bool Grid::enabled_ = true;

int Grid::gridSize()
{
    return gridSize_;
}

void Grid::setGridSize(int gridSize)
{
    Q_ASSERT(gridSize >= minGridSize() && gridSize <= maxGridSize());
    gridSize_ = gridSize;
}

int Grid::minGridSize()
{
    return 10;
}

int Grid::maxGridSize()
{
    return 100;
}

bool Grid::enabled()
{
    return enabled_;
}

void Grid::setEnabled(bool enabled)
{
    enabled_ = enabled;
}

QPointF Grid::snapToGrid(const QPointF &point, int gridSize)
{
    return QPointF(qRound(point.x() / gridSize) * gridSize,
                   qRound(point.y() / gridSize) * gridSize);
}
