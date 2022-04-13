#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QPointF>

enum class MoveOrientation : int
{
    LeftRight,
    UpDown
};

class Waypoint
{
public:
    explicit Waypoint(const QPointF& pos = QPointF(),
                      MoveOrientation moveOrientation = MoveOrientation::LeftRight);
    QPointF pos() const;
    void setPos(const QPointF &pos);

    MoveOrientation moveOrientation() const;
    void setMoveOrientation(const MoveOrientation &moveOrientation);

private:
    QPointF pos_;
    MoveOrientation moveOrientation_;
};

#endif // WAYPOINT_H
