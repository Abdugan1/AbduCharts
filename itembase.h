#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <QGraphicsItem>

class ItemBase : public QGraphicsItem
{
public:
    struct Data {
        qreal x = 0.0f;
        qreal y = 0.0f;
        QMargins margins;
    };

    explicit ItemBase(QGraphicsItem* parent = nullptr);
    explicit ItemBase(const Data& data, QGraphicsItem* parent = nullptr);

    virtual QRectF contentRect() const = 0;

    const QMarginsF &margins() const;
    void setMargins(const QMarginsF &newMargins);

    void applyData(const Data& data);

private:
    QMarginsF margins_;
};

QPointF snapToGrid(const QPointF& point, int gridSize);

#endif // ITEMBASE_H
