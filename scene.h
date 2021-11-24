#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QtCore/qglobal.h>

class FlowchartItem;
class GuideLine;
class PositionLine;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    explicit Scene(const QRectF& sceneRect, QObject* parent = nullptr);

    void addItem(FlowchartItem* item);
    void removeItem(FlowchartItem* item);

signals:
    void itemSelected(FlowchartItem* item);
    void itemLostSelection(FlowchartItem* item);
    void itemMoved(FlowchartItem* item);

private slots:
    void onItemMoved(FlowchartItem* movedItem);
    void bringToFront(FlowchartItem* item);
    void deleteAllGuidelines();

private:
    void addPositionLine(PositionLine* positionLine);

private:
    QList<FlowchartItem*> items_;
    QList<GuideLine*> guideLines_;
};

#endif // SCENE_H
