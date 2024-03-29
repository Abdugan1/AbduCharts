#include "editor/itemresizer.h"
#include "editor/items/flowchartshapeitem.h"
#include "editor/resizehandle.h"

#include <QDebug>

ItemResizer::ItemResizer(FlowchartShapeItem *targetResizeItem, QObject *parent)
    : QObject(parent)
    , targetResizeItem_(targetResizeItem)
{
}

void ItemResizer::setCompareRect(const QRectF &newCompareRect)
{
    compareRect_ = newCompareRect;
}

void ItemResizer::resize(const QRectF &newRect)
{
    oldCompareRect_ = compareRect_;
    compareRect_ = newRect;
    if (compareRect_ != oldCompareRect_)
        doResize();
}

void ItemResizer::onHandleMoved(ResizeHandle *resizeHandle, qreal dx, qreal dy)
{
    updateCompareRect(resizeHandle, dx, dy);
    if (compareRect_ != oldCompareRect_) {
        doResize();
        emit resizeBeenMadeByUser(oldCompareRect_, compareRect_);
    }
}

void ItemResizer::updateCompareRect(const ResizeHandle *resizeHandle, qreal dx, qreal dy)
{
    oldCompareRect_ = compareRect_;
    switch (resizeHandle->positionFlags()) {
    case ResizeHandle::TopLeft:
        compareRect_.setTopLeft(QPointF(compareRect_.left() + dx, compareRect_.top() + dy));
        break;
    case ResizeHandle::Top:
        compareRect_.setTop(compareRect_.top() + dy);
        break;
    case ResizeHandle::TopRight:
        compareRect_.setTopRight(QPointF(compareRect_.right() + dx, compareRect_.top() + dy));
        break;
    case ResizeHandle::Left:
        compareRect_.setLeft(compareRect_.left() + dx);
        break;
    case ResizeHandle::Right:
        compareRect_.setRight(compareRect_.right() + dx);
        break;
    case ResizeHandle::BottomLeft:
        compareRect_.setBottomLeft(QPointF(compareRect_.left() + dx, compareRect_.bottom() + dy));
        break;
    case ResizeHandle::Bottom:
        compareRect_.setBottom(compareRect_.bottom() + dy);
        break;
    case ResizeHandle::BottomRight:
        compareRect_.setBottomRight(QPointF(compareRect_.right() + dx, compareRect_.bottom() + dy));
        break;
    }
}

void ItemResizer::doResize()
{
    targetResizeItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    qreal sx = compareRect_.width()  / targetResizeItem_->contentRect().width();
    qreal sy = compareRect_.height() / targetResizeItem_->contentRect().height();

    QPainterPath oldShape = targetResizeItem_->shape();
    QPainterPath newShape = oldShape * QTransform::fromScale(sx, sy);

    targetResizeItem_->setShape(newShape);

    qreal dx = compareRect_.x() - targetResizeItem_->contentRect().x();
    qreal dy = compareRect_.y() - targetResizeItem_->contentRect().y();

    targetResizeItem_->moveBy(dx, dy);

    compareRect_ = targetResizeItem_->contentRect();
    targetResizeItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
