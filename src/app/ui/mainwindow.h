#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Scene;
class View;
class UpperToolBar;
class BottomToolBar;
class ItemLibraryDockWidget;
class QTextCharFormat;
class QTextBlockFormat;
class FlowchartTextItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
    void deleteActionTriggered();

public slots:
    void onCharFormatChanged(const QTextCharFormat& format);
    void onBlockFormatChanged(const QTextBlockFormat& format);

private slots:
    void updateUpperToolbarFormattingResponsiblePart(FlowchartTextItem* textItem);

private:
    void init();
    void initConnections();
    void initActions();
    void initMenus();
    void initUi();

private:
    Scene* scene_ = nullptr;
    View*  view_  = nullptr;

    UpperToolBar* upperToolBar_ = nullptr;
    BottomToolBar* bottomToolBar_ = nullptr;

    ItemLibraryDockWidget* itemLibraryDockWidget_ = nullptr;

    QMenu* editMenu_ = nullptr;
    QAction* deleteAction_ = nullptr;
};

#endif // MAINWINDOW_H
