#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

class Scene;
class FlowchartShapeItem;

class QLabel;

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View(Scene *scene, QWidget *parent = nullptr);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QWheelEvent* event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent* event) override;

private slots:
    void showAndUpdateItemInfoLabels(FlowchartShapeItem* selectedItem);
    void hideItemInfoLabels();
    void updateItemInfoLabels(FlowchartShapeItem* selectedItem);

    void scalingTime(qreal x);
    void animFinished();

private:
    void init();
    void initFlags();
    void initSomething();
    void initLayout();
    void initConnection();

    void updateSelectedItemPositionLabel(FlowchartShapeItem* selectedItem);
    void updateSelectedItemFigureTypeLabel(FlowchartShapeItem* selectedItem);

    void updateCursorPositionLabel(const QPoint& pos);

private:
    Scene* scene_ = nullptr;

    QLabel* cursorPositionLabel_         = nullptr;

    // Selected item info labels
    QLabel* selectedItemFigureTypeLabel_ = nullptr;
    QLabel* selectedItemPositionLabel_   = nullptr;

    int numScheduledScalings_ = 0;
};

#endif // VIEW_H
