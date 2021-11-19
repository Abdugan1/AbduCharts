#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QGraphicsTextItem>
#include <QtCore/qglobal.h>

class TextItem : public QGraphicsTextItem
{
public:
    enum AlignFlags {
        Left,
        Center,
        Right
    };

    explicit TextItem(QGraphicsItem* parent = nullptr);
    ~TextItem();

private slots:
    void updateAlignment();

private:
    AlignFlags alignment_ = AlignFlags::Center;
};



class MyItem;

class MyTextItem : public TextItem
{
public:
    explicit MyTextItem(MyItem* parent = nullptr);
    ~MyTextItem();

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
    MyItem* myItem_ = nullptr;
    bool acceptMousePress_ = false;
};

#endif // TEXTITEM_H
