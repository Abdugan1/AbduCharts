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

public slots:
    void onHandleMoved(ResizeHandle* resizeHandle, qreal dx, qreal dy);

protected:

private:
    void updateCompareRect(ResizeHandle* resizeHandle, qreal dx, qreal dy);
    void doResize();

private:
    MyItem* targetResizeItem_ = nullptr;
    QRectF compareRect_;
};

#endif // ITEMRESIZER_H
