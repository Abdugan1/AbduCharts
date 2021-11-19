#ifndef ITEMRESIZER_H
#define ITEMRESIZER_H

#include <QObject>
#include <QRectF>

class ResizeHandle;
class MyItem;

class ItemResizer : public QObject
{
    Q_OBJECT
public:
    explicit ItemResizer(MyItem* targetResizeItem, QObject *parent = nullptr);

signals:
    void resizeBeenMade();

public slots:
    void onHandleMoved(ResizeHandle* resizeHandle, qreal dx, qreal dy);

private:
    void updateCompareRect(const ResizeHandle* resizeHandle, qreal dx, qreal dy);
    void doResize();
    void limitResize();

private:
    MyItem* targetResizeItem_ = nullptr;
    QRectF compareRect_;
    QRectF oldCompareRect_;

    qreal minWidth_  = 0.0f;
    qreal minHegith_ = 0.0f;
    qreal maxWidth_  = INFINITY;
    qreal maxHeight_ = INFINITY;
};

#endif // ITEMRESIZER_H
