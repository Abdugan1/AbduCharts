#include "ui/bottomtoolbar.h"
#include "ui/zoomvalidator.h"
#include "ui/zoomcombobox.h"

#include <QSlider>
#include <QToolButton>
#include <QComboBox>
#include <QAbstractItemView>
#include <QLineEdit>
#include <QBoxLayout>
#include <QDebug>

const int ScaleStep = 5;

BottomToolBar::BottomToolBar(QWidget *parent)
    : QToolBar(parent)
    , zoomSlider_(new QSlider)
    , zoomOutButton_(new QToolButton)
    , zoomInButton_(new QToolButton)
    , zoomCombo_(new ZoomComboBox)
{
    zoomSlider_->setOrientation(Qt::Horizontal);
    zoomSlider_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    zoomSlider_->setMinimum(10);
    zoomSlider_->setMaximum(400);
    zoomSlider_->setValue(100);

    const int size = 18;
    zoomOutButton_->setFixedSize(size, size);
    zoomOutButton_->setIcon(QIcon(":/images/minus.png"));

    zoomInButton_->setFixedSize(size, size);
    zoomInButton_->setIcon(QIcon(":/images/plus.png"));


    connect(zoomSlider_, &QSlider::sliderMoved, this, &BottomToolBar::emitZoomValueConvertedToScale);

    connect(zoomCombo_, &ZoomComboBox::zoomValueChanged, this, &BottomToolBar::emitZoomValueConvertedToScale);

    connect(zoomOutButton_, &QToolButton::clicked, this, [this]() {
        int value = zoomSlider_->value() - ScaleStep;
        value = qMax(value, zoomSlider_->minimum());
        emitZoomValueConvertedToScale(value);
    });

    connect(zoomInButton_, &QToolButton::clicked, this, [this]() {
        int value = zoomSlider_->value() + ScaleStep;
        value = qMin(value, zoomSlider_->maximum());
        emitZoomValueConvertedToScale(value);
    });

    QHBoxLayout* layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Maximum));
    layout->addWidget(zoomOutButton_);
    layout->addWidget(zoomSlider_);
    layout->addWidget(zoomInButton_);
    layout->addWidget(zoomCombo_);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    addWidget(widget);

    setMovable(false);
}

void BottomToolBar::updateZoomIndicators(qreal scale)
{
    zoomSlider_->setValue(qRound(scale * 100.0));
    zoomCombo_->setZoomValue(qRound(scale * 100.0));
}

void BottomToolBar::emitZoomValueConvertedToScale(int zoomValue)
{
    qreal scale = zoomValue / 100.0;
    emit zoomChanged(scale);
}
