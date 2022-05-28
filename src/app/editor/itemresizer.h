#ifndef ITEMRESIZER_H
#define ITEMRESIZER_H

#include <QObject>
#include <QRectF>
#include <QtMath>

class ResizeHandle;
class FlowchartShapeItem;

class ItemResizer : public QObject
{
    Q_OBJECT
public:
    explicit ItemResizer(FlowchartShapeItem* targetResizeItem, QObject *parent = nullptr);

    void setCompareRect(const QRectF &newCompareRect);

    void resize(const QRectF& newRect);

signals:
    void resizeBeenMadeByUser(const QRectF& oldRect, const QRectF& currentRect);

public slots:
    void onHandleMoved(ResizeHandle* resizeHandle, qreal dx, qreal dy);

private:
    void updateCompareRect(const ResizeHandle* resizeHandle, qreal dx, qreal dy);
    void doResize();

private:
    FlowchartShapeItem* targetResizeItem_ = nullptr;
    QRectF compareRect_;
    QRectF oldCompareRect_;

    qreal minWidth_  = 0.0f;
    qreal minHegith_ = 0.0f;
    qreal maxWidth_  = INFINITY;
    qreal maxHeight_ = INFINITY;
};

#endif // ITEMRESIZER_H
