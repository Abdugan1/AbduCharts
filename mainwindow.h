#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Scene;
class View;
class UpperToolBar;
class QTextCharFormat;
class QTextBlockFormat;
class FlowchartTextItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void onCharFormatChanged(const QTextCharFormat& format);
    void onBlockFormatChanged(const QTextBlockFormat& format);

private slots:
    void updateUpperToolbarFormattingResponsiblePart(FlowchartTextItem* textItem);

private:
    Scene* scene_ = nullptr;
    View*  view_  = nullptr;

    UpperToolBar* upperToolBar_ = nullptr;
};

#endif // MAINWINDOW_H
