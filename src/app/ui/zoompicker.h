#ifndef ZOOMPICKER_H
#define ZOOMPICKER_H

#include <QWidget>

class ZoomLineEdit;
class QListWidget;

class ZoomPicker : public QWidget
{
    Q_OBJECT
public:
    explicit ZoomPicker(QWidget *parent = nullptr);

    void setZoomValue(int zoomValue);

signals:
    void zoomValueChanged(int);

protected:
    void showEvent(QShowEvent *event) override;

private:
    ZoomLineEdit* lineEdit_  = nullptr;
    QListWidget* listWidget_ = nullptr;
};

#endif // ZOOMPICKER_H
