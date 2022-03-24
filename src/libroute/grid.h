#ifndef GRID_H
#define GRID_H

#include <QMap>

class QRectF;

class Grid
{
public:
    explicit Grid();

    void set(int row, int col, const QRectF& rect);
    QRectF get(int row, int col) const;


    // TODO change data container or something else
    int rows() const;
    int cols() const;

private:
    QMap<int, QMap<int, QRectF>> data_;
};

#endif // GRID_H
