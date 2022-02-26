#include "zoomcombobox.h"
#include "zoompicker.h"

#include <QToolButton>
#include <QBoxLayout>
#include <QApplication>
#include <QDesktopWidget>

ZoomComboBox::ZoomComboBox(QWidget *parent)
    : QWidget(parent)
    , expandListButton_(new QToolButton)
    , zoomPicker_(new ZoomPicker)
{
    expandListButton_->setText("100 %");
    expandListButton_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    expandListButton_->setArrowType(Qt::DownArrow);

    connect(expandListButton_, &QToolButton::clicked, this, &ZoomComboBox::showZoomPicker);

    connect(zoomPicker_.get(), &ZoomPicker::zoomValueChanged, this, &ZoomComboBox::zoomValueChanged);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(expandListButton_);

    setLayout(layout);
}

void ZoomComboBox::setZoomValue(int zoomValue)
{
    expandListButton_->setText(QString::number(zoomValue) + " %");
    zoomPicker_->setZoomValue(zoomValue);
}

void ZoomComboBox::showZoomPicker() const
{
    QPoint pos = getAllowableZoomPickerShowPos();
    zoomPicker_->move(pos);
    zoomPicker_->show();
}

QPoint ZoomComboBox::getAllowableZoomPickerShowPos() const
{
    QPoint pos = mapToGlobal(expandListButton_->frameGeometry().bottomLeft());
    QRect desktopAvailableGeometry = qApp->desktop()->availableGeometry();

    if ((pos.y() + zoomPicker_->height()) > desktopAvailableGeometry.bottom())
        pos.setY(desktopAvailableGeometry.bottom() - zoomPicker_->height());

    if (pos.x() + zoomPicker_->width() > desktopAvailableGeometry.right())
        pos.setX(desktopAvailableGeometry.right() - zoomPicker_->width());

    return pos;
}
