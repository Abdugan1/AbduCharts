#ifndef BOTTOMTOOLBAR_H
#define BOTTOMTOOLBAR_H

#include <QToolBar>

class QSlider;
class QPushButton;
class QComboBox;
class ZoomComboBox;

class BottomToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit BottomToolBar(QWidget* parent = nullptr);

signals:
    void zoomChanged(qreal zoomRatio);

public slots:
    void updateZoomIndicators(qreal scale);

private slots:
    void emitZoomValueConvertedToScale(int zoomValue);

private:
    QSlider* zoomSlider_ = nullptr;
    QPushButton* zoomOutButton_ = nullptr;
    QPushButton* zoomInButton_  = nullptr;
    ZoomComboBox* zoomCombo_ = nullptr;
};

#endif // BOTTOMTOOLBAR_H
