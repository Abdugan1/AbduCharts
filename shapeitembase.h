#ifndef SHAPEITEMBASE_H
#define SHAPEITEMBASE_H

#include <QGraphicsItem>

class ShapeItemBase : public QGraphicsItem
{
public:
    enum { Type = UserType + 1 };

    struct Data {
        qreal x = 0.0f;
        qreal y = 0.0f;
        QMargins margins;
    };

    explicit ShapeItemBase(QGraphicsItem* parent = nullptr);
    explicit ShapeItemBase(const Data& data, QGraphicsItem* parent = nullptr);

    virtual QRectF contentRect() const = 0;

    int type() const override;

    const QMarginsF &margins() const;
    void setMargins(const QMarginsF &newMargins);

    void applyData(const Data& data);

private:
    QMarginsF margins_;
};

QPointF snapToGrid(const QPointF& point, int gridSize);

#endif // SHAPEITEMBASE_H
