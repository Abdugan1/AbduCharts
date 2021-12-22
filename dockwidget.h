#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>

class QFrame;
class QGridLayout;

class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(QWidget* parent = nullptr);

private:
    QFrame* frame_ = nullptr;
    QGridLayout* gridLayout_ = nullptr;
};

#endif // DOCKWIDGET_H
