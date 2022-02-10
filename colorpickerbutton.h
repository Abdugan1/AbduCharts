#ifndef COLORPICKERBUTTON_H
#define COLORPICKERBUTTON_H

#include <QToolButton>

class ColorPickerButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ColorPickerButton(const QColor& initColor = Qt::white, QWidget* parent = nullptr);

signals:
    void colorPicked(QColor color);

private slots:
    void onClicked();

private:
    void updateIconColor();

private:
    QColor color_;
};

#endif // COLORPICKERBUTTON_H
