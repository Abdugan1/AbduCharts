#include "ui/zoompicker.h"
#include "ui/zoomlineedit.h"

#include <QListWidget>
#include <QLabel>
#include <QBoxLayout>

ZoomPicker::ZoomPicker(QWidget *parent)
    : QWidget(parent)
    , lineEdit_(new ZoomLineEdit)
    , listWidget_(new QListWidget)
{
    listWidget_->addItems({"400 %", "200 %", "150 %", "125 %", "100 %", "75 %", "50 %", "25 %"});

    connect(listWidget_, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        QString text = item->text();
        QString onlyDigits = text.left(text.indexOf(' '));
        hide();
        emit zoomValueChanged(onlyDigits.toInt());
    });

    connect(lineEdit_, &ZoomLineEdit::editingFinished, this, [this]() {
        emit zoomValueChanged(lineEdit_->text().toInt());
    });

    QLabel* percentLabel = new QLabel("Percent:");

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(percentLabel);
    hLayout->addWidget(lineEdit_);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    vLayout->addWidget(listWidget_);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
    setFixedSize(130, 165);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);


#ifdef Q_OS_LINUX
    show();
    hide();
#endif
}

void ZoomPicker::setZoomValue(int zoomValue)
{
    lineEdit_->setText(QString::number(zoomValue));
}

void ZoomPicker::showEvent(QShowEvent *event)
{
    lineEdit_->setFocus();
    QWidget::showEvent(event);
}
