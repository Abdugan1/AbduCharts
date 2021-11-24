#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QGraphicsTextItem>
#include <QtCore/qglobal.h>

class TextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit TextItem(QGraphicsItem* parent = nullptr);
    ~TextItem();

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment newAlignment);

private slots:
    void updateAlignment();

private:
    Qt::Alignment alignment_;
};


class FlowchartItem;

class FlowchartTextItem : public TextItem
{
    Q_OBJECT
public:
    explicit FlowchartTextItem(FlowchartItem* parent = nullptr);
    ~FlowchartTextItem();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

public slots:
    void enableTextEditing();
    void disableTextEditing();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void centerOnMyItem();

private:
    FlowchartItem* myItem_ = nullptr;
    bool acceptMousePress_ = false;
};

#endif // TEXTITEM_H
