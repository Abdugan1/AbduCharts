#include "myitem.h"
#include "resizehandle.h"
#include "itemresizer.h"

#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>

constexpr qreal Width  = 200.0f;
constexpr qreal Height = 80.0f;

void drawCross(QPainter* painter, const QPointF& point)
{
    painter->save();
    painter->setPen(Qt::red);
    painter->drawLine(QLineF(point.x(), point.y() - 5,
                             point.x(), point.y() + 5));
    painter->drawLine(QLineF(point.x() - 5, point.y(),
                             point.x() + 5, point.y()));
    painter->restore();
}

MyItem::MyItem(QGraphicsItem *parent)
    : ItemBase(parent)
{
    setFlags(ItemSendsGeometryChanges
             | ItemIsMovable
             | ItemIsSelectable);
    setMargins(QMarginsF(5, 5, 5, 5));
    setAcceptedMouseButtons(Qt::LeftButton);

    QRectF rect(-Width / 2, -Height / 2, Width, Height);
    QPolygonF polygon;
    polygon << QPointF(rect.left(),
                       rect.top()  + rect.height() / 2)
            << QPointF(rect.left() + rect.width()  / 2,
                       rect.top())
            << QPointF(rect.right(),
                       rect.top()  + rect.height() / 2)
            << QPointF(rect.left() + rect.width()  / 2,
                       rect.bottom())
            << QPointF(rect.left(),
                       rect.top()  + rect.height() / 2);

    shape_.addPolygon(polygon);

    resizer_ = new ItemResizer(this);

    auto addResizeHandle = [this](ResizeHandle::PositionFlags positionFlags) {
        auto resizeHandle = new ResizeHandle(positionFlags, this);
        connect(resizeHandle, &ResizeHandle::moved, this, &MyItem::onResizeHandleMoved);
        resizeHandles_.append(resizeHandle);
    };

    addResizeHandle(ResizeHandle::TopLeft    );
    addResizeHandle(ResizeHandle::Top        );
    addResizeHandle(ResizeHandle::TopRight   );
    addResizeHandle(ResizeHandle::Left       );
    addResizeHandle(ResizeHandle::Right      );
    addResizeHandle(ResizeHandle::BottomLeft );
    addResizeHandle(ResizeHandle::Bottom     );
    addResizeHandle(ResizeHandle::BottomRight);

    updateResizeHandlesPositions();
}

MyItem::~MyItem()
{
    qDebug() << "Bye World!";
}

QRectF MyItem::boundingRect() const
{
    return contentRect() + margins();
}

QRectF MyItem::contentRect() const
{
    return shape_.boundingRect();
}

QPainterPath MyItem::shape() const
{
    return shape_;
}

void MyItem::setShape(const QPainterPath &shape)
{
    shape_ = shape;
}

void MyItem::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    painter->setPen(Qt::red);
    painter->drawRect(boundingRect());
    painter->setPen(Qt::blue);
    painter->drawRect(contentRect());
    painter->setPen(option->state & QStyle::State_Selected ? Qt::green : Qt::black);
    painter->setBrush(Qt::white);
    painter->drawPath(shape_);
    drawCross(painter, QPointF(0, 0));
}

void MyItem::onResizeHandleMoved(ResizeHandle *resizeHandle, qreal dx, qreal dy)
{
    prepareGeometryChange();
    resizer_->onHandleMoved(resizeHandle, dx, dy);
    updateResizeHandlesPositions();
}

QVariant MyItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
        return snapToGrid(value.toPointF(), 20);
    } else if (change == ItemPositionHasChanged) {
        emit moved(this);
    } else if (change == ItemSelectedHasChanged) {
        if (value.toBool()) {
            emit selected(this);
        } else {
            emit lostSelection(this);
        }
    }
    return ItemBase::itemChange(change, value);
}

void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit released();
    ItemBase::mouseReleaseEvent(event);
}

void MyItem::updateResizeHandlesPositions()
{
    auto setHandlePos = [](ResizeHandle* resizeHandle, const QPointF& pos) {
        resizeHandle->setPos(pos);
    };

    for (auto handle : resizeHandles_) {
        switch (handle->positionFlags()) {
        case ResizeHandle::TopLeft:
            setHandlePos(handle, QPointF(contentRect().left(),
                                         contentRect().top()));
            break;
        case ResizeHandle::Top:
            setHandlePos(handle, QPointF(contentRect().left() + contentRect().width() / 2,
                                         contentRect().top()));
            break;
        case ResizeHandle::TopRight:
            setHandlePos(handle, QPointF(contentRect().right(),
                                         contentRect().top()));
            break;
        case ResizeHandle::Left:
            setHandlePos(handle, QPointF(contentRect().left(),
                                         contentRect().top() + contentRect().height() / 2));
            break;
        case ResizeHandle::Right:
            setHandlePos(handle, QPointF(contentRect().right(),
                                         contentRect().top() + contentRect().height() / 2));
            break;
        case ResizeHandle::BottomLeft:
            setHandlePos(handle, QPointF(contentRect().left(),
                                         contentRect().bottom()));
            break;
        case ResizeHandle::Bottom:
            setHandlePos(handle, QPointF(contentRect().left() + contentRect().width() / 2,
                                         contentRect().bottom()));
            break;
        case ResizeHandle::BottomRight:
            setHandlePos(handle, QPointF(contentRect().right(),
                                         contentRect().bottom()));
            break;
        }
    }
}

