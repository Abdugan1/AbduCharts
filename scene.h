#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QtCore/qglobal.h>

class MyItem;
class GuideLine;
class PositionLine;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    explicit Scene(const QRectF& sceneRect, QObject* parent = nullptr);

    void addItem(MyItem* item);
    void removeItem(MyItem* item);

private slots:
    void onItemMoved(MyItem* movedItem);
    void bringToFront(MyItem* item);
    void deleteAllGuidelines();

private:
    void addPositionLine(PositionLine* positionLine);

private:
    QList<MyItem*> items_;
    QList<GuideLine*> guideLines_;
};

#endif // SCENE_H
