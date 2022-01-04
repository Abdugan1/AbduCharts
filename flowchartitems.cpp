#include "flowchartitems.h"
#include "resizehandle.h"
#include "itemresizer.h"
#include "textitem.h"

#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QTextCursor>
#include <QDebug>

int map(int value, int frowLow, int fromHigh, int toLow, int toHigh)
{
    return (value - frowLow) * (toHigh - toLow) / (fromHigh - frowLow) + toLow;
}

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

FlowchartShapeItem::FlowchartShapeItem(QGraphicsItem *parent)
    : ShapeItemBase(parent)
{
    setFlags(ItemSendsGeometryChanges
             | ItemIsMovable
             | ItemIsSelectable);
    setMargins(QMarginsF(5, 5, 5, 5));
    setAcceptedMouseButtons(Qt::LeftButton);

    resizer_ = new ItemResizer(this);
    connect(resizer_, &ItemResizer::resizeBeenMade, this, &FlowchartShapeItem::resized);

    textItem_ = new FlowchartShapesTextItem(this);
    connect(this, &FlowchartShapeItem::lostSelection, textItem_, &FlowchartShapesTextItem::disableTextEditingAndMousePress);

    addResizeHandle(ResizeHandle::TopLeft    );
    addResizeHandle(ResizeHandle::Top        );
    addResizeHandle(ResizeHandle::TopRight   );
    addResizeHandle(ResizeHandle::Left       );
    addResizeHandle(ResizeHandle::Right      );
    addResizeHandle(ResizeHandle::BottomLeft );
    addResizeHandle(ResizeHandle::Bottom     );
    addResizeHandle(ResizeHandle::BottomRight);
}

FlowchartShapeItem::~FlowchartShapeItem()
{
    qDebug() << "Bye World!";
}

QRectF FlowchartShapeItem::boundingRect() const
{
    return contentRect() + margins();
}

QRectF FlowchartShapeItem::contentRect() const
{
    return shape_.boundingRect();
}

QPainterPath FlowchartShapeItem::shape() const
{
    return shape_;
}

void FlowchartShapeItem::setShape(const QPainterPath &shape)
{
    shape_ = shape;
}

void FlowchartShapeItem::paint(QPainter *painter,
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

void FlowchartShapeItem::setText(const QString &text)
{
    textItem_->setPlainText(text);
}

void FlowchartShapeItem::onResizeHandleMoved(ResizeHandle *resizeHandle, qreal dx, qreal dy)
{
    prepareGeometryChange();
    resizer_->onHandleMoved(resizeHandle, dx, dy);
    updateResizeHandlesPositions();
}

QVariant FlowchartShapeItem::itemChange(GraphicsItemChange change, const QVariant &value)
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
    return ShapeItemBase::itemChange(change, value);
}

void FlowchartShapeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (textEditingEnabled())
        textItem_->disableTextEditingAndMousePress();

    ShapeItemBase::mousePressEvent(event);
}

void FlowchartShapeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit released();
    ShapeItemBase::mouseReleaseEvent(event);
}

void FlowchartShapeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    textItem_->enableTextEditingAndMousePress();
}

void FlowchartShapeItem::addResizeHandle(ResizeHandle::PositionFlags positionFlags)
{
    auto resizeHandle = new ResizeHandle(positionFlags, this);
    connect(resizeHandle, &ResizeHandle::moved, this, &FlowchartShapeItem::onResizeHandleMoved);
    resizeHandles_.append(resizeHandle);
}

void FlowchartShapeItem::updateResizeHandlesPositions() const
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

void FlowchartShapeItem::initShape_SetResizerRect_UpdateHandlePositions(const QPainterPath &shape)
{
    setShape(shape);
    resizer_->setCompareRect(contentRect());
    updateResizeHandlesPositions();
}

bool FlowchartShapeItem::textEditingEnabled() const
{
    return (textItem_->textInteractionFlags() == Qt::TextEditorInteraction);
}

FlowchartShapesTextItem *FlowchartShapeItem::textItem() const
{
    return textItem_;
}

//------------------------------------------------
//                Terminal
//------------------------------------------------

Terminal::Terminal(QGraphicsItem *parent)
    : FlowchartShapeItem(parent)
{
    QRectF rect(-Width / 2, -Height / 2, Width, Height);
    QPainterPath shape;
    shape.addRoundedRect(rect, Height / 2, Height / 2);
    initShape_SetResizerRect_UpdateHandlePositions(shape);

    setText("Begin");
}

QString Terminal::figureType() const
{
    return tr("Terminal");
}

//------------------------------------------------
//                Process
//------------------------------------------------

Process::Process(QGraphicsItem *parent)
    : FlowchartShapeItem(parent)
{
    QRectF rect(-Width / 2, -Height / 2, Width, Height);
    QPainterPath shape;
    shape.addRect(rect);
    initShape_SetResizerRect_UpdateHandlePositions(shape);
}

QString Process::figureType() const
{
    return tr("Process");
}


//------------------------------------------------
//                Decision
//------------------------------------------------

Decision::Decision(QGraphicsItem *parent)
    : FlowchartShapeItem(parent)
{
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
    QPainterPath shape;
    shape.addPolygon(polygon);
    initShape_SetResizerRect_UpdateHandlePositions(shape);
}

QString Decision::figureType() const
{
    return tr("Decision");
}

//------------------------------------------------
//                InOut
//------------------------------------------------

InOut::InOut(QGraphicsItem *parent)
    : FlowchartShapeItem(parent)
{
    QRectF rect(-Width / 2, -Height / 2, Width, Height);
    QPolygonF polygon;
    polygon << QPointF(rect.left() + Width / 4, rect.top()) << QPointF(rect.topRight())
            << QPointF(rect.right() - Width / 4, rect.bottom()) << rect.bottomLeft()
            << QPointF(rect.left() + Width / 4, rect.top());
    QPainterPath shape;

    shape.addPolygon(polygon);
    initShape_SetResizerRect_UpdateHandlePositions(shape);

    setText("Input/Output");
}

QString InOut::figureType() const
{
    return tr("Input/Output");
}
