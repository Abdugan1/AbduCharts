#include "dockwidget.h"
#include "itembutton.h"
#include "flowchartitems.h"

#include <QFrame>
#include <QGridLayout>

DockWidget::DockWidget(QWidget *parent)
    : QDockWidget(parent)
    , frame_(new QFrame)
    , gridLayout_(new QGridLayout)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setWindowTitle("Flowchart Shapes");

    gridLayout_->addWidget(new ItemButton(Terminal()),  0, 0);
    gridLayout_->addWidget(new ItemButton(Process()),   0, 1);
    gridLayout_->addWidget(new ItemButton(Decision()),  0, 2);
    gridLayout_->addWidget(new ItemButton(InOut()),     0, 3);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(gridLayout_);
    vLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    frame_->setLayout(vLayout);
    setWidget(frame_);
}
