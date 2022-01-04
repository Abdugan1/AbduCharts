#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QGraphicsTextItem>
#include <QTextCharFormat>

class FlowchartTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit FlowchartTextItem(QGraphicsItem* parent = nullptr);
    ~FlowchartTextItem();

    void mergeCharFormat(const QTextCharFormat& format);
    void setBlockFormat(const QTextBlockFormat& format);

    void setTextCursor(const QTextCursor& cursor);

    void enableTextEditing();
    void disableTextEditing();

signals:
    void enabled(FlowchartTextItem* textItem);
    void disabled(FlowchartTextItem* textItem);

    void currentCharFormatChanged(const QTextCharFormat& format);
    void currentBlockFormatChanged(const QTextBlockFormat& format);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event)  override;

    void keyPressEvent(QKeyEvent* event) override;

private:
    bool isKeyThatCanChangeCursorPositionPressed(QKeyEvent* event) const;
    bool isArrowKeyPressed(int key) const;

    void mergeTextFormatToSelection(QTextCursor* cursor, const QTextCharFormat& format);
    void mergeTextFormatToWordUnderCursor(QTextCursor* cursor, const QTextCharFormat& format);

    void emitCurrentFormattingChangedIfNecessary();

    bool isCharFormatChanged() const;
    bool isBlockFormatChanged() const;

    void setDefaultFormat();

private:
    QTextCharFormat  lastCharFormat_;
    QTextBlockFormat lastBlockFormat_;

    static QTextCharFormat  defaultCharFormat_;
    static QTextBlockFormat defaultBlockFormat_;
};


class FlowchartShapeItem;

class FlowchartShapesTextItem : public FlowchartTextItem
{
    Q_OBJECT
public:
    explicit FlowchartShapesTextItem(FlowchartShapeItem* parent = nullptr);
    ~FlowchartShapesTextItem();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

public slots:
    void enableTextEditingAndMousePress();
    void disableTextEditingAndMousePress();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void centerOnShapeItem();

private:
    FlowchartShapeItem* myItem_ = nullptr;
    bool acceptMousePress_ = false;
};


class TextItem : public FlowchartTextItem
{
    Q_OBJECT
public:
    explicit TextItem(QGraphicsItem* parent = nullptr);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // TEXTITEM_H
