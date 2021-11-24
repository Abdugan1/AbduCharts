#include <QtWidgets>

#include "flowchartitems.h"
#include "scene.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const qreal SceneWidth  = 1000.0f;
    const qreal SceneHeight = 1000.0f;

    Scene* scene = new Scene({-SceneWidth / 2, -SceneHeight / 2, SceneWidth, SceneHeight});
    View view(scene);

    FlowchartItem* myItem = new Decision;
    scene->addItem(myItem);

    FlowchartItem* myItem2 = new Process;
    scene->addItem(myItem2);

    FlowchartItem* myItem3 = new Terminal;
    scene->addItem(myItem3);

    FlowchartItem* myItem4 = new InOut;
    scene->addItem(myItem4);

//    QTimer::singleShot(2000, [=](){
//        scene->removeItem(myItem2);
//    });

    view.show();

    return app.exec();
}
