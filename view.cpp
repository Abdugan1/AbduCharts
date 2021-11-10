#include "view.h"

#include <QWheelEvent>
#include <QTimeLine>
#include <QDebug>

View::View(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setViewportUpdateMode(FullViewportUpdate);
}

void View::drawBackground(QPainter *painter, const QRectF &rect)
{
    constexpr int gridSize = 20;
    int left = int(rect.left() - (int(rect.left()) % gridSize));
    int top  = int(rect.top()  - (int(rect.top())  % gridSize));

    QVarLengthArray<QPoint> points; // It works faster than reserving. IDK why
    for (int x = left; x < rect.right(); x += gridSize) {
        for (int y = top; y < rect.bottom(); y += gridSize) {
            points.append(QPoint(x, y));
        }
    }

    painter->drawPoints(points.data(), points.size());

    QGraphicsView::drawBackground(painter, rect);
}

void View::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::CTRL) {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;
        numScheduledScalings_ += numSteps;
        if (numScheduledScalings_ * numSteps < 0)
            numScheduledScalings_ = numSteps;

        QTimeLine* anim = new QTimeLine(350, this);
        anim->setUpdateInterval(20);
        connect(anim, &QTimeLine::valueChanged, this, &View::scalingTime);
        connect(anim, &QTimeLine::finished, this, &View::animFinished);
        anim->start();

    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void View::scalingTime(qreal x)
{
    qreal factor = 1.0f + qreal(numScheduledScalings_) / 300.0f;
    scale(factor, factor);
}

void View::animFinished()
{
    if (numScheduledScalings_ > 0) {
        --numScheduledScalings_;
    } else {
        ++numScheduledScalings_;
        sender()->deleteLater();
    }
}
