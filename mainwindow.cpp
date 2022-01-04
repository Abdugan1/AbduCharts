#include "mainwindow.h"
#include "scene.h"
#include "view.h"
#include "flowchartitems.h"
#include "textitem.h"

#include "uppertoolbar.h"
#include "dockwidget.h"

#include <QTextCursor>
#include <QDebug>

const qreal SceneWidth  = 1000.0f;
const qreal SceneHeight = 1000.0f;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , scene_(new Scene({-SceneWidth / 2, -SceneHeight / 2, SceneWidth, SceneHeight}))
    , view_(new View(scene_))
    , upperToolBar_(new UpperToolBar)
{
    connect(upperToolBar_, &UpperToolBar::charFormatChanged,
            this,          &MainWindow::onCharFormatChanged);

    connect(upperToolBar_, &UpperToolBar::blockFormatChanged,
            this,          &MainWindow::onBlockFormatChanged);

    connect(scene_,        &Scene::currentCharFormatChanged,
            upperToolBar_, &UpperToolBar::updateCharFormattingResponsiblePart);

    connect(scene_,        &Scene::currentBlockFormatChanged,
            upperToolBar_, &UpperToolBar::updateBlockFormattingResponsiblePart);

    connect(scene_, &Scene::switchedToAnotherTextItem,
            this,   &MainWindow::updateUpperToolbarFormattingResponsiblePart);

    addToolBar(upperToolBar_);
    addDockWidget(Qt::LeftDockWidgetArea, new DockWidget);
    setCentralWidget(view_);
}

void MainWindow::onCharFormatChanged(const QTextCharFormat &format)
{
    view_->setFocus(Qt::OtherFocusReason);
    scene_->applyCharFormatOnCurrentTextItem(format);
}

void MainWindow::onBlockFormatChanged(const QTextBlockFormat &format)
{
    view_->setFocus(Qt::OtherFocusReason);
    scene_->applyBlockFormatOnCurrentTextItem(format);
}

void MainWindow::updateUpperToolbarFormattingResponsiblePart(FlowchartTextItem *textItem)
{
    qDebug() << "updating!!!";
    QTextCharFormat  charFormat = textItem->textCursor().charFormat();
    QTextBlockFormat blockFormat = textItem->textCursor().blockFormat();
    upperToolBar_->updateCharFormattingResponsiblePart(charFormat);
    upperToolBar_->updateBlockFormattingResponsiblePart(blockFormat);
}
