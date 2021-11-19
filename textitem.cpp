#include "textitem.h"
#include "myitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QDebug>

TextItem::TextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setFont(QFont("Times New Roman", 14));

    connect(document(), &QTextDocument::contentsChange,
            this,       &TextItem::updateAlignment);

}

TextItem::~TextItem()
{
    qDebug() << "Bye TextItem!";
}

void TextItem::updateAlignment()
{
    qDebug() << "Updating :D";
}

MyTextItem::MyTextItem(MyItem *parent)
    : TextItem(parent)
    , myItem_(parent)
{
    setHtml("<center>This is a text!</center>");
    centerOnMyItem();

    connect(parent, &MyItem::resized, this, &MyTextItem::centerOnMyItem);
    connect(document(), &QTextDocument::contentsChanged, this, &MyTextItem::centerOnMyItem);
}

MyTextItem::~MyTextItem()
{
    qDebug() << "Bye MyTextItem!";
}

void MyTextItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    TextItem::paint(painter, option, widget);
}

void MyTextItem::enableTextEditing()
{
    acceptMousePress_ = true;
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFocus(Qt::MouseFocusReason);
    setSelected(true);
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    setTextCursor(cursor);
}

void MyTextItem::disableTextEditing()
{
    acceptMousePress_ = false;
    setTextInteractionFlags(Qt::NoTextInteraction);
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
    clearFocus();
}

void MyTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "MyTextItem: mousePress";
    if (!acceptMousePress_)
        event->ignore();
    else
        TextItem::mousePressEvent(event);
}

void MyTextItem::centerOnMyItem()
{
    QRectF textRect   = boundingRect();
    setPos(-textRect.width() / 2, -textRect.height() / 2);
}
