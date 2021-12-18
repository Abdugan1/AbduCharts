#include "textitem.h"
#include "flowchartitems.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QCursor>
#include <QDebug>

QTextCharFormat FlowchartTextItem::futureItemsFormat_ = QTextCharFormat();

FlowchartTextItem::FlowchartTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    futureItemsFormat_.setFontFamily("Arial");
    futureItemsFormat_.setFontPointSize(20);

    connect(document(), &QTextDocument::contentsChange,
            this,       &FlowchartTextItem::updateAlignment);
}

FlowchartTextItem::~FlowchartTextItem()
{
    qDebug() << "Bye TextItem!";
}

void FlowchartTextItem::updateAlignment()
{
    QPointF topRightPrev = boundingRect().topRight();
    setTextWidth(-1);
    setTextWidth(boundingRect().width());
    setAlignment(alignment_);
    QPointF topRight = boundingRect().topRight();

    if (alignment_ & Qt::AlignRight)
        setPos(pos() + (topRightPrev - topRight));
}

void FlowchartTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mousePressEvent(event);
    emitCurrentCharFormatChangedIfNecessary();
}

void FlowchartTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseMoveEvent(event);

    emitCurrentCharFormatChangedIfNecessary();
}

void FlowchartTextItem::keyPressEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyPressEvent(event);

    if (isKeyThatCanChangeCursorPositionPressed(event)) {
        emitCurrentCharFormatChangedIfNecessary();
    }
}

bool FlowchartTextItem::isKeyThatCanChangeCursorPositionPressed(QKeyEvent *event) const
{
    return (isArrowKeyPressed(event->key())
            || (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_A)
            || (event->key() == Qt::Key_Home)
            || (event->key() == Qt::Key_End));
}

bool FlowchartTextItem::isArrowKeyPressed(int key) const
{
    return (key == Qt::Key_Left  || key == Qt::Key_Right
            || key == Qt::Key_Up || key == Qt::Key_Down);
}

void FlowchartTextItem::mergeTextFormatToSelection(QTextCursor *cursor, const QTextCharFormat &format)
{
    cursor->mergeCharFormat(format);
}

void FlowchartTextItem::mergeTextFormatToWordUnderCursor(QTextCursor *cursor, const QTextCharFormat &format)
{
    QTextCursor temp = *cursor;
    cursor->select(QTextCursor::WordUnderCursor);
    if (cursor->selectedText().isEmpty()) {
        cursor->swap(temp);
        cursor->mergeCharFormat(format);
    } else {
        int prevPos = cursor->position();
        cursor->mergeCharFormat(lastCharFormat_);
        cursor->clearSelection();
        cursor->setPosition(prevPos);
    }
}

void FlowchartTextItem::emitCurrentCharFormatChangedIfNecessary()
{
    if (lastCharFormat_ != textCursor().charFormat()) {
        lastCharFormat_ = textCursor().charFormat();
        emit currentCharFormatChanged(lastCharFormat_);
    }
}

Qt::Alignment FlowchartTextItem::alignment() const
{
    return alignment_;
}

void FlowchartTextItem::setAlignment(Qt::Alignment newAlignment)
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

void FlowchartTextItem::mergeTextFormat(const QTextCharFormat &format)
{
    lastCharFormat_.merge(format);
    setTextFormatForAllFutureItems(format);

    QTextCursor cursor = textCursor();
    if (cursor.hasSelection()) {
        mergeTextFormatToSelection(&cursor, format);
    } else {
        mergeTextFormatToWordUnderCursor(&cursor, format);
    }

    setTextCursor(cursor);
}

void FlowchartTextItem::setTextFormatForAllFutureItems(const QTextCharFormat &format)
{
    futureItemsFormat_ = format;
}

void FlowchartTextItem::setTextCursor(const QTextCursor &cursor)
{
    QGraphicsTextItem::setTextCursor(cursor);
    emitCurrentCharFormatChangedIfNecessary();
}

void FlowchartTextItem::enableTextEditing()
{
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFocus(Qt::MouseFocusReason);
    setSelected(true);
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    setTextCursor(cursor);

    setCursor(Qt::IBeamCursor);

    emit enabled(this);
}

void FlowchartTextItem::disableTextEditing()
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
    clearFocus();

    setCursor(Qt::ArrowCursor);

    emit disabled(this);
}


//----------------------------------------------------------------
//                  FlowchartShapesTextItem
//----------------------------------------------------------------


FlowchartShapesTextItem::FlowchartShapesTextItem(FlowchartShapeItem *parent)
    : FlowchartTextItem(parent)
    , myItem_(parent)
{
    centerOnMyItem();
    setAlignment(Qt::AlignCenter);

    connect(parent, &FlowchartShapeItem::resized, this, &FlowchartShapesTextItem::centerOnMyItem);
    connect(document(), &QTextDocument::contentsChanged, this, &FlowchartShapesTextItem::centerOnMyItem);
}

FlowchartShapesTextItem::~FlowchartShapesTextItem()
{
    qDebug() << "Bye MyTextItem!";
}

void FlowchartShapesTextItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    FlowchartTextItem::paint(painter, option, widget);
}

void FlowchartShapesTextItem::enableTextEditingAndMousePress()
{
    acceptMousePress_ = true;
    enableTextEditing();
}

void FlowchartShapesTextItem::disableTextEditingAndMousePress()
{
    acceptMousePress_ = false;
    disableTextEditing();
}

void FlowchartShapesTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "MyTextItem: mousePress";
    if (!acceptMousePress_) {
        event->ignore();
    } else {
        FlowchartTextItem::mousePressEvent(event);
    }
}

void FlowchartShapesTextItem::centerOnMyItem()
{
    setPos(-boundingRect().width() / 2, -boundingRect().height() / 2);
}


//----------------------------------------------------------------
//                  TextItem
//----------------------------------------------------------------


TextItem::TextItem(QGraphicsItem *parent)
    : FlowchartTextItem(parent)
{
    setPlainText("Im a TextItem!!!");
}

void TextItem::focusOutEvent(QFocusEvent *event)
{
    qDebug() << "focusOutEvent";
    FlowchartTextItem::focusOutEvent(event);
}
