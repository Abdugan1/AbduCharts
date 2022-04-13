#include "waypoint.h"

Waypoint::Waypoint(const QPointF &pos, MoveOrientation moveOrientation)
    : pos_(pos)
    , moveOrientation_(moveOrientation)
{
}

QPointF Waypoint::pos() const
{
    return pos_;
}

void Waypoint::setPos(const QPointF &pos)
{
    pos_ = pos;
}

MoveOrientation Waypoint::moveOrientation() const
{
    return moveOrientation_;
}

void Waypoint::setMoveOrientation(const MoveOrientation &moveOrientation)
{
    moveOrientation_ = moveOrientation;
}
