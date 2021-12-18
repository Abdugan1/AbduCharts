#include "mainwindow.h"
#include "scene.h"
#include "view.h"
#include "flowchartitems.h"
#include "textitem.h"

#include "uppertoolbar.h"

#include <QTextCursor>

const qreal SceneWidth  = 1000.0f;
const qreal SceneHeight = 1000.0f;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , scene_(new Scene({-SceneWidth / 2, -SceneHeight / 2, SceneWidth, SceneHeight}))
    , view_(new View(scene_))
    , upperToolBar_(new UpperToolBar)
{
    View view(scene_);

    FlowchartShapeItem* myItem = new Decision;
    scene_->addItem(myItem);

    FlowchartShapeItem* myItem2 = new Process;
    scene_->addItem(myItem2);

    FlowchartShapeItem* myItem3 = new Terminal;
    scene_->addItem(myItem3);

    FlowchartShapeItem* myItem4 = new InOut;
    scene_->addItem(myItem4);

    connect(upperToolBar_, &UpperToolBar::fontFormatChanged,
            this,          &MainWindow::onTextFormatChanged);

    connect(scene_,        &Scene::currentCharFormatChanged,
            upperToolBar_, &UpperToolBar::updateTextFormattingResponsiblePart);

    connect(scene_, &Scene::switchedToAnotherTextItem, [this](FlowchartTextItem* textItem) {
        QTextCharFormat format = textItem->textCursor().charFormat();
        upperToolBar_->updateTextFormattingResponsiblePart(format);
    });

    addToolBar(upperToolBar_);
    setCentralWidget(view_);
}

void MainWindow::onTextFormatChanged(const QTextCharFormat &format)
{
    view_->setFocus(Qt::OtherFocusReason);
    scene_->applyFormatOnCurrentTextItem(format);
}

