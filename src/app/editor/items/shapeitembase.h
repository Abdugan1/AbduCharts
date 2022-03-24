#ifndef SHAPEITEMBASE_H
#define SHAPEITEMBASE_H

#include <QGraphicsItem>

class ShapeItemBase : public QGraphicsItem
{
public:
    enum { Type = UserType + 1 };

    explicit ShapeItemBase(QGraphicsItem* parent = nullptr);
    ~ShapeItemBase();

    virtual QRectF contentRect() const = 0;

    int type() const override;

    const QMarginsF &margins() const;
    void setMargins(const QMarginsF &newMargins);

private:
    QMarginsF margins_;
};

#endif // SHAPEITEMBASE_H
