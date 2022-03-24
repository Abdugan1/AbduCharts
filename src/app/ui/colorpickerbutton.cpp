#include "ui/colorpickerbutton.h"

#include <QColorDialog>
#include <QPainter>
#include <QDebug>

ColorPickerButton::ColorPickerButton(const QColor &initColor, QWidget *parent)
    : QToolButton(parent)
    , color_(initColor)
{
    connect(this, &ColorPickerButton::clicked, this, &ColorPickerButton::onClicked);
    setFixedSize(48, 24);
    setIconSize(size());
    updateIconColor();
}

void ColorPickerButton::onClicked()
{
    QColorDialog colorDialog;
    colorDialog.setWindowTitle(tr("Select grid color"));

    QColor color = QColorDialog::getColor();
    if (color.isValid()) {
        color_ = color;
        updateIconColor();
        emit colorPicked(color);
    }
}

void ColorPickerButton::updateIconColor()
{
    const int margin = 3;
    QPixmap pixmap(width(), height());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setBrush(color_);
    painter.drawRect(margin, margin, width() - margin * 2, height() - margin * 2);
    setIcon(pixmap);
}
