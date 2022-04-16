#include "ui/itemlibrarydockkwidget.h"
#include "ui/itembutton.h"
#include "editor/items/flowchartshapeitems.h"

#include <QFrame>
#include <QGridLayout>

ItemLibraryDockWidget::ItemLibraryDockWidget(QWidget *parent)
    : QDockWidget(parent)
    , frame_(new QFrame)
    , gridLayout_(new QGridLayout)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setWindowTitle("Flowchart Shapes");

    gridLayout_->addWidget(new ItemButton(TerminalShapeItem::Type),  0, 0);
    gridLayout_->addWidget(new ItemButton(ProcessShapeItem::Type),   0, 1);
    gridLayout_->addWidget(new ItemButton(DecisionShapeItem::Type),  0, 2);
    gridLayout_->addWidget(new ItemButton(InOutShapeItem::Type),     0, 3);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(gridLayout_);
    vLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    frame_->setLayout(vLayout);
    setWidget(frame_);
}
