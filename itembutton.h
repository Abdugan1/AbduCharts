#ifndef ITEMBUTTON_H
#define ITEMBUTTON_H

#include <QToolButton>

class FlowchartShapeItem;

class ItemButton : public QToolButton
{
public:
    explicit ItemButton(const FlowchartShapeItem& shapeItem, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QString figureType_;

    QPoint dragStartPosition_;
};

#endif // ITEMBUTTON_H
