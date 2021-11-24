#include "textitem.h"
#include "flowchartitems.h"

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
    QPointF topRightPrev = boundingRect().topRight();
    setTextWidth(-1);
    setTextWidth(boundingRect().width());
    setAlignment(alignment_);
    QPointF topRight = boundingRect().topRight();

    if (alignment_ & Qt::AlignRight)
        setPos(pos() + (topRightPrev - topRight));
}

Qt::Alignment TextItem::alignment() const
{
    return alignment_;
}

void TextItem::setAlignment(Qt::Alignment newAlignment)
{
    alignment_ = newAlignment;
    QTextBlockFormat format;
    format.setAlignment(newAlignment);
    QTextCursor cursor = textCursor();
    int position = textCursor().position();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    cursor.setPosition(position);
    setTextCursor(cursor);
}

FlowchartTextItem::FlowchartTextItem(FlowchartItem *parent)
    : TextItem(parent)
    , myItem_(parent)
{
    centerOnMyItem();
    setAlignment(Qt::AlignCenter);

    connect(parent, &FlowchartItem::resized, this, &FlowchartTextItem::centerOnMyItem);
    connect(document(), &QTextDocument::contentsChanged, this, &FlowchartTextItem::centerOnMyItem);
}

FlowchartTextItem::~FlowchartTextItem()
{
    qDebug() << "Bye MyTextItem!";
}

void FlowchartTextItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    TextItem::paint(painter, option, widget);
}

void FlowchartTextItem::enableTextEditing()
{
    acceptMousePress_ = true;
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFocus(Qt::MouseFocusReason);
    setSelected(true);
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    setTextCursor(cursor);
}

void FlowchartTextItem::disableTextEditing()
{
    acceptMousePress_ = false;
    setTextInteractionFlags(Qt::NoTextInteraction);
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
    clearFocus();
}

void FlowchartTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "MyTextItem: mousePress";
    if (!acceptMousePress_)
        event->ignore();
    else
        TextItem::mousePressEvent(event);
}

void FlowchartTextItem::centerOnMyItem()
{
    QRectF textRect   = boundingRect();
    setPos(-textRect.width() / 2, -textRect.height() / 2);
}
