#ifndef ITEMBUTTON_H
#define ITEMBUTTON_H

#include <QToolButton>

class FlowchartShapeItem;

class ItemButton : public QToolButton
{
public:
    explicit ItemButton(int itemType, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QString figureType_;
    int itemType_ = -1;

    QPoint dragStartPosition_;
};

#endif // ITEMBUTTON_H
