#include "grid.h"

#include <QRectF>

Grid::Grid()
{
}

void Grid::set(int row, int col, const QRectF &rect)
{
    data_[row][col] = rect;
}

QRectF Grid::get(int row, int col) const
{
    return data_[row][col];
}

int Grid::rows() const
{
    return data_.size();
}

int Grid::cols() const
{
    return data_[0].size();
}
