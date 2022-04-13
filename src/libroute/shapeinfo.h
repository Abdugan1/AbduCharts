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

class ShapeInfo
{
public:
    explicit ShapeInfo(QGraphicsItem* item = nullptr,
                       ConnectionSide connectionSide = ConnectionSide::Top);
    QGraphicsItem *item() const;
    void setItem(QGraphicsItem *item);

    ConnectionSide connectionSide() const;
    void setConnectionSide(const ConnectionSide &connectionSide);

private:
    QGraphicsItem* item_ = nullptr;
    ConnectionSide connectionSide_;

};

#endif // SHAPEINFO_H
