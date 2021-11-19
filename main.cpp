#include <QtWidgets>

#include "myitem.h"
#include "scene.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    constexpr qreal SceneWidth  = 1000.0f;
    constexpr qreal SceneHeight = 1000.0f;

    Scene* scene = new Scene({-SceneWidth / 2, -SceneHeight / 2, SceneWidth, SceneHeight});
    View view(scene);

    MyItem* myItem = new MyItem;
    scene->addItem(myItem);

    MyItem* myItem2 = new MyItem;
    scene->addItem(myItem2);

    MyItem* myItem3 = new MyItem;
    scene->addItem(myItem3);

//    QTimer::singleShot(2000, [=](){
//        scene->removeItem(myItem2);
//    });

    view.show();

    return app.exec();
}
