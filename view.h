#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QtCore/qglobal.h>

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QWheelEvent* event) override;

private slots:
    void scalingTime(qreal x);
    void animFinished();

private:
    int numScheduledScalings_ = 0;
};

#endif // VIEW_H
