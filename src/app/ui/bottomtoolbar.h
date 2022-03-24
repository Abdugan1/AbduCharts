#ifndef BOTTOMTOOLBAR_H
#define BOTTOMTOOLBAR_H

#include <QToolBar>

class QSlider;
class QToolButton;
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
    QToolButton* zoomOutButton_ = nullptr;
    QToolButton* zoomInButton_  = nullptr;
    ZoomComboBox* zoomCombo_ = nullptr;
};

#endif // BOTTOMTOOLBAR_H
