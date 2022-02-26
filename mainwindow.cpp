#include "mainwindow.h"
#include "scene.h"
#include "view.h"
#include "flowchartitems.h"
#include "textitems.h"

#include "uppertoolbar.h"
#include "bottomtoolbar.h"
#include "itemlibrarydockkwidget.h"

#include <QTextCursor>
#include <QDebug>

const qreal SceneWidth  = 1000.0f;
const qreal SceneHeight = 1000.0f;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , scene_(new Scene({-SceneWidth / 2, -SceneHeight / 2, SceneWidth, SceneHeight}, this))
    , view_(new View(scene_))
    , upperToolBar_(new UpperToolBar)
    , bottomToolBar_(new BottomToolBar)
    , itemLibraryDockWidget_(new ItemLibraryDockWidget)
{
    // UpperToolBar connect
    connect(upperToolBar_, &UpperToolBar::charFormatChanged,
            this,          &MainWindow::onCharFormatChanged);

    connect(upperToolBar_, &UpperToolBar::blockFormatChanged,
            this,          &MainWindow::onBlockFormatChanged);

    connect(upperToolBar_,     &UpperToolBar::gridSizeValueChanged,
            view_->viewport(), QOverload<>::of(&QWidget::update));

    connect(upperToolBar_, &UpperToolBar::gridColorChanged,
            view_,         &View::updateGridColor);

    connect(upperToolBar_,     &UpperToolBar::gridEnabledChanged,
            view_->viewport(), QOverload<>::of(&QWidget::update));

    // BottomToolBar connect
    connect(bottomToolBar_, &BottomToolBar::zoomChanged,
            view_,          &View::zoom);

    // Scene connect
    connect(scene_,        &Scene::currentCharFormatChanged,
            upperToolBar_, &UpperToolBar::updateCharFormattingResponsiblePart);

    connect(scene_,        &Scene::currentBlockFormatChanged,
            upperToolBar_, &UpperToolBar::updateBlockFormattingResponsiblePart);

    connect(scene_, &Scene::switchedToAnotherTextItem,
            this,   &MainWindow::updateUpperToolbarFormattingResponsiblePart);

    connect(scene_, &Scene::itemMovedAndReleased,
            view_,  &View::addToUndoStackMoveCommand);

    // View connect
    connect(view_,          &View::scaleChanged,
            bottomToolBar_, &BottomToolBar::updateZoomIndicators);

    addToolBar(Qt::TopToolBarArea, upperToolBar_);
    addToolBar(Qt::BottomToolBarArea, bottomToolBar_);

    addDockWidget(Qt::LeftDockWidgetArea, itemLibraryDockWidget_);
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
