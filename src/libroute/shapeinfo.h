#ifndef SHAPEINFO_H
#define SHAPEINFO_H

class QGraphicsItem;

enum class ConnectionSide : int
{
    Top,
    Left,
    Bottom,
    Right
};

struct ShapeInfo
{
    QGraphicsItem* item = nullptr;
    ConnectionSide side;

};

#endif // SHAPEINFO_H
