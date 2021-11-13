#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

class QLabel;
class Scene;
class MyItem;

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
    void scalingTime(qreal x);
    void animFinished();

private:
    void init();
    void initFlags();
    void initSomething();
    void initLayout();
    void initConnection();

    void updateSelectedItemPositionLabel(MyItem* selectedItem);

private:
    Scene* scene_ = nullptr;

    QLabel* cursorPositionLabel_        = nullptr;
    QLabel* selectedItemPositionLabel_  = nullptr;

    int numScheduledScalings_ = 0;
};

#endif // VIEW_H
