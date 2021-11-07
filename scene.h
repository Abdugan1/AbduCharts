#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QtCore/qglobal.h>

class Scene : public QGraphicsScene
{
public:
    explicit Scene(QObject *parent = nullptr);
};

#endif // SCENE_H
