#ifndef FLOWCHARTTEXTITEM_H
#define FLOWCHARTTEXTITEM_H


#include <QGraphicsTextItem>
#include <QTextCharFormat>

class FlowchartTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum {Type = UserType + 1000};

    explicit FlowchartTextItem(QGraphicsItem* parent = nullptr);
    ~FlowchartTextItem();

    int type() const override;

    void mergeCharFormat(const QTextCharFormat& format);
    void setBlockFormat(const QTextBlockFormat& format);

    void setTextCursor(const QTextCursor& cursor);

    void enableTextEditing();
    void disableTextEditing();

signals:
    void enabled(FlowchartTextItem* textItem);
    void disabled(FlowchartTextItem* textItem);

    void pressed(FlowchartTextItem* textItem);
    void released(FlowchartTextItem* textItem);

    void currentCharFormatChanged(const QTextCharFormat& format);
    void currentBlockFormatChanged(const QTextBlockFormat& format);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event)   override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event)    override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

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

#endif // FLOWCHARTTEXTITEM_H
