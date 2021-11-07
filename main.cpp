#include <QtWidgets>

#include "myitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGraphicsScene* scene = new QGraphicsScene;
    QGraphicsView view(scene);

    MyItem* myItem = new MyItem;
    scene->addItem(myItem);


    view.show();

    return app.exec();
}
