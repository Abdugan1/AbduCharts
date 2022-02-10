#ifndef GRID_H
#define GRID_H

#include <QPointF>

class Grid
{
public:
    static int gridSize();
    static void setGridSize(int gridSize);

    static int minGridSize();
    static int maxGridSize();

    static bool enabled();
    static void setEnabled(bool enabled);

    static QPointF snapToGrid(const QPointF& point, int gridSize = gridSize_);

private:
    static int gridSize_;
    static bool enabled_;
};

#endif // GRID_H
