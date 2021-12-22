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

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment newAlignment);

    void mergeTextFormat(const QTextCharFormat& format);
    static void setTextFormatForAllFutureItems(const QTextCharFormat& format);

    void setTextCursor(const QTextCursor& cursor);

    void enableTextEditing();
    void disableTextEditing();

signals:
    void enabled(FlowchartTextItem* textItem);
    void disabled(FlowchartTextItem* textItem);

    void currentCharFormatChanged(const QTextCharFormat& format);

private slots:
    void updateAlignment();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event)  override;

    void keyPressEvent(QKeyEvent* event) override;

private:
    bool isKeyThatCanChangeCursorPositionPressed(QKeyEvent* event) const;
    bool isArrowKeyPressed(int key) const;

    void mergeTextFormatToSelection(QTextCursor* cursor, const QTextCharFormat& format);
    void mergeTextFormatToWordUnderCursor(QTextCursor* cursor, const QTextCharFormat& format);

    void emitCurrentCharFormatChangedIfNecessary();

private:
    Qt::Alignment alignment_;

    QTextCharFormat lastCharFormat_;
    static QTextCharFormat futureItemsFormat_;
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
    void centerOnMyItem();

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
