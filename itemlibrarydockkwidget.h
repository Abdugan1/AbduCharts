#ifndef ITEMLIBRARYDOCKKWIDGET_H
#define ITEMLIBRARYDOCKKWIDGET_H

#include <QDockWidget>

class QFrame;
class QGridLayout;

class ItemLibraryDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit ItemLibraryDockWidget(QWidget* parent = nullptr);

private:
    QFrame* frame_ = nullptr;
    QGridLayout* gridLayout_ = nullptr;
};

#endif // ITEMLIBRARYDOCKKWIDGET_H
