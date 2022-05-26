#include "flowcharttextitem.h"
#include "editor/items/flowchartshapeitem.h"
#include "editor/grid.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QCursor>
#include <QDebug>

QTextCharFormat FlowchartTextItem::defaultCharFormat_ = QTextCharFormat();
QTextBlockFormat FlowchartTextItem::defaultBlockFormat_ = QTextBlockFormat();

FlowchartTextItem::FlowchartTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setDefaultFormat();

    // Without this align wont work
    connect(document(), &QTextDocument::contentsChange, this, [this]() {
        setTextWidth(-1);
        setTextWidth(document()->idealWidth());
    });
}

FlowchartTextItem::~FlowchartTextItem()
{
    qDebug() << "FlowchartTextItem: Destructor";
}

int FlowchartTextItem::type() const
{
    return Type;
}

void FlowchartTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit pressed(this);
    QGraphicsTextItem::mousePressEvent(event);
    emitCurrentFormattingChangedIfNecessary();
}

void FlowchartTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseMoveEvent(event);

    emitCurrentFormattingChangedIfNecessary();
}

void FlowchartTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit released(this);
    QGraphicsTextItem::mouseReleaseEvent(event);
}

void FlowchartTextItem::keyPressEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyPressEvent(event);

    if (isKeyThatCanChangeCursorPositionPressed(event)) {
        emitCurrentFormattingChangedIfNecessary();
    }
}

bool FlowchartTextItem::isKeyThatCanChangeCursorPositionPressed(QKeyEvent *event) const
{
    return (isArrowKeyPressed(event->key())
            || (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_A)
            || (event->key() == Qt::Key_Home)
            || (event->key() == Qt::Key_End)
            || (QKeySequence(event->modifiers() + event->key()).matches(QKeySequence::Undo)
                == QKeySequence::ExactMatch)
            || (QKeySequence(event->modifiers() + event->key()).matches(QKeySequence::Redo)
                == QKeySequence::ExactMatch));
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
    QTextCursor newCursor = *cursor;
    int previousPosition = newCursor.position();

    newCursor.select(QTextCursor::WordUnderCursor);
    newCursor.mergeCharFormat(format);
    newCursor.setPosition(previousPosition);

    setTextCursor(newCursor);

    QFont font = newCursor.charFormat().font();
    qDebug() << "Position:" << cursor->position();
    qDebug() << "Family:" << font.family();
    qDebug() << "Size:" << font.pointSize();
    qDebug() << "Bold?" << font.bold();
    qDebug() << "Italic?" << font.italic();
    qDebug() << "Underline?" << font.underline();
}

void FlowchartTextItem::emitCurrentFormattingChangedIfNecessary()
{
    if (isCharFormatChanged()) {
        lastCharFormat_ = textCursor().charFormat();
        emit currentCharFormatChanged(lastCharFormat_);
    } else if (isBlockFormatChanged()) {
        lastBlockFormat_ = textCursor().blockFormat();
        emit currentBlockFormatChanged(lastBlockFormat_);
    }
}

bool FlowchartTextItem::isCharFormatChanged() const
{
    return lastCharFormat_ != textCursor().charFormat();
}

bool FlowchartTextItem::isBlockFormatChanged() const
{
    return lastBlockFormat_ != textCursor().blockFormat();
}

void FlowchartTextItem::setDefaultFormat()
{
    QTextOption option;
    option.setAlignment(defaultBlockFormat_.alignment());
    QFont font(defaultCharFormat_.font());

    document()->setDefaultTextOption(option);
    document()->setDefaultFont(font);
}

void FlowchartTextItem::mergeCharFormat(const QTextCharFormat &format)
{
    lastCharFormat_.merge(format);

    QTextCursor cursor(textCursor());
    if (cursor.hasSelection()) {
        mergeTextFormatToSelection(&cursor, format);
    } else {
        mergeTextFormatToWordUnderCursor(&cursor, format);
    }

    defaultCharFormat_.merge(format);
}

void FlowchartTextItem::setBlockFormat(const QTextBlockFormat &format)
{
    QTextCursor cursor(textCursor());
    cursor.setBlockFormat(format);

    lastBlockFormat_ = format;
    defaultBlockFormat_ = format;
}

void FlowchartTextItem::setTextCursor(const QTextCursor &cursor)
{
    qDebug() << "FlowchartTextItem: Custom setTextCursor called";
    QGraphicsTextItem::setTextCursor(cursor);
    emitCurrentFormattingChangedIfNecessary();
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
