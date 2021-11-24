#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

class Scene;
class FlowchartItem;

class QLabel;
class QToolButton;

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View(Scene *scene, QWidget *parent = nullptr);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void showAndUpdateItemInfoLabels(FlowchartItem* selectedItem);
    void hideItemInfoLabels();
    void updateItemInfoLabels(FlowchartItem* selectedItem);

    void scalingTime(qreal x);
    void animFinished();

private:
    void init();
    void initFlags();
    void initSomething();
    void initLayout();
    void initConnection();

    void updateSelectedItemPositionLabel(FlowchartItem* selectedItem);
    void updateSelectedItemFigureTypeLabel(FlowchartItem* selectedItem);

    void updateCursorPositionLabel(const QPoint& pos);

private:
    Scene* scene_ = nullptr;

    QLabel* cursorPositionLabel_         = nullptr;

    // Selected item info labels
    QLabel* selectedItemFigureTypeLabel_ = nullptr;
    QLabel* selectedItemPositionLabel_   = nullptr;

    QToolButton* changeFlowchartButton_  = nullptr;

    int numScheduledScalings_ = 0;
};

#endif // VIEW_H
