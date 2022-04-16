#include "ui/mainwindow.h"
#include "ui/uppertoolbar.h"
#include "ui/bottomtoolbar.h"
#include "ui/itemlibrarydockkwidget.h"
#include "editor/scene.h"
#include "editor/view.h"
#include "editor/items/flowchartshapeitem.h"
#include "editor/items/flowcharttextitem.h"

#include <QTextCursor>
#include <QMenuBar>
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
    init();
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
    qDebug() << "MainWindow: Updating toolbars text format part";
    QTextCharFormat  charFormat = textItem->textCursor().charFormat();
    QTextBlockFormat blockFormat = textItem->textCursor().blockFormat();
    upperToolBar_->updateCharFormattingResponsiblePart(charFormat);
    upperToolBar_->updateBlockFormattingResponsiblePart(blockFormat);
}

void MainWindow::init()
{
    initConnections();
    initActions();
    initMenus();
    initUi();
}

void MainWindow::initConnections()
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

    connect(upperToolBar_,     &UpperToolBar::gridEnableStateChanged,
            view_->viewport(), QOverload<>::of(&QWidget::update));

    connect(upperToolBar_,  &UpperToolBar::viewBackgroundEnableStateChanged,
            view_,          &View::setBackgroundEnabled);

    connect(upperToolBar_,  &UpperToolBar::viewBackgroundColorChanged,
            view_,          &View::updateBackgroundColor);

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

    connect(this,  &MainWindow::deleteActionTriggered,
            view_, &View::deleteSelectedItem);
}

void MainWindow::initActions()
{
    deleteAction_ = new QAction("&Delete", this);
    deleteAction_->setShortcut(QKeySequence::Delete);
    connect(deleteAction_, &QAction::triggered, this, [this]() {
        qDebug() << "Edit->Delete action triggered";
        emit deleteActionTriggered();
    });
}

void MainWindow::initMenus()
{
    editMenu_ = menuBar()->addMenu("&Edit");
    editMenu_->addAction(deleteAction_);
}

void MainWindow::initUi()
{
    addToolBar(Qt::TopToolBarArea, upperToolBar_);
    addToolBar(Qt::BottomToolBarArea, bottomToolBar_);

    addDockWidget(Qt::LeftDockWidgetArea, itemLibraryDockWidget_);
    setCentralWidget(view_);
}
