#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class FlowchartShapeItem;
class GuideLine;
class PositionLine;
class FlowchartTextItem;

class QTextCharFormat;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    explicit Scene(const QRectF& sceneRect, QObject* parent = nullptr);

    void addItem(FlowchartShapeItem* item);
    void removeItem(FlowchartShapeItem* item);

signals:
    void itemSelected(FlowchartShapeItem* item);
    void itemLostSelection(FlowchartShapeItem* item);
    void itemMoved(FlowchartShapeItem* item);

    void currentCharFormatChanged(const QTextCharFormat& format);
    void switchedToAnotherTextItem(FlowchartTextItem* textItem);

public slots:
    void applyFormatOnCurrentTextItem(const QTextCharFormat& format);

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
    QList<GuideLine*> guideLines_;
};

#endif // SCENE_H
