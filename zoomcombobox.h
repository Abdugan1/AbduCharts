#ifndef ZOOMCOMBOBOX_H
#define ZOOMCOMBOBOX_H

#include <QWidget>
#include <memory>

class QToolButton;
class ZoomPicker;

class ZoomComboBox : public QWidget
{
    Q_OBJECT
public:
    explicit ZoomComboBox(QWidget *parent = nullptr);

    void setZoomValue(int zoomValue);

signals:
    void zoomValueChanged(int zoomValue);

private slots:
    void showZoomPicker() const;

private:
    QPoint getAllowableZoomPickerShowPos() const;

private:
    QToolButton* expandListButton_ = nullptr;
    std::shared_ptr<ZoomPicker> zoomPicker_ = nullptr;
};

#endif // ZOOMCOMBOBOX_H
