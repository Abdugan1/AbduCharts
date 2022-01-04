#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class FlowchartShapeItem;
class GuideLine;
class PositionLine;
class FlowchartTextItem;

class QTextCharFormat;
class QTextBlockFormat;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    explicit Scene(const QRectF& sceneRect, QObject* parent = nullptr);

    void addItem(FlowchartShapeItem* item);
    void addItem(FlowchartTextItem* item);
    void removeItem(FlowchartShapeItem* item);

signals:
    void itemSelected(FlowchartShapeItem* item);
    void itemLostSelection(FlowchartShapeItem* item);
    void itemMoved(FlowchartShapeItem* item);

    void currentCharFormatChanged(const QTextCharFormat& format);
    void currentBlockFormatChanged(const QTextBlockFormat& format);
    void switchedToAnotherTextItem(FlowchartTextItem* textItem);

public slots:
    void applyCharFormatOnCurrentTextItem(const QTextCharFormat& format);
    void applyBlockFormatOnCurrentTextItem(const QTextBlockFormat& format);

private slots:
    void onItemMoved(FlowchartShapeItem* movedItem);
    void bringToFront(FlowchartShapeItem* item);

    void deleteAllGuidelines();

private:
    void addPositionLine(PositionLine* positionLine);

    void connectSignalsOfShapeItem(FlowchartShapeItem* item);
    void connectSignalsOfTextItem(FlowchartTextItem* textItem);

private:
    QList<FlowchartShapeItem*> flowchartShapeItems_;
    QList<FlowchartTextItem*> textItems_;
    QList<GuideLine*> guideLines_;
};

#endif // SCENE_H
