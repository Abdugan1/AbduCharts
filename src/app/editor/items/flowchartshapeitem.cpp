#include "editor/items/flowchartshapeitem.h"
#include "editor/resizehandle.h"
#include "editor/itemresizer.h"
#include "editor/connectorhandle.h"
#include "editor/connectoritemmanager.h"
#include "editor/textitems.h"
#include "editor/grid.h"


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


FlowchartShapeItem::FlowchartShapeItem(QGraphicsItem *parent)
    : ShapeItemBase(parent)
{
    setFlags(ItemSendsGeometryChanges
             | ItemIsMovable
             | ItemIsSelectable);
    setMargins(QMarginsF(5, 5, 5, 5));
    setAcceptedMouseButtons(Qt::LeftButton);
    setCursor(Qt::SizeAllCursor);

    resizer_ = new ItemResizer(this);
    connect(resizer_, &ItemResizer::resizeBeenMade, this,
            [this] (const QRectF& oldRect, const QRectF& currentRect) {
        emit resizedByHands(this, oldRect, currentRect);
    });

    textItem_ = new FlowchartShapesTextItem(this);
    connect(this, &FlowchartShapeItem::lostSelection, textItem_,
            &FlowchartShapesTextItem::disableTextEditingAndMousePress);

    addResizeHandle(ResizeHandle::TopLeft    );
    addResizeHandle(ResizeHandle::Top        );
    addResizeHandle(ResizeHandle::TopRight   );
    addResizeHandle(ResizeHandle::Left       );
    addResizeHandle(ResizeHandle::Right      );
    addResizeHandle(ResizeHandle::BottomLeft );
    addResizeHandle(ResizeHandle::Bottom     );
    addResizeHandle(ResizeHandle::BottomRight);

    for (auto resizeHandle : qAsConst(resizeHandles_)) {
        connect(resizeHandle, &ResizeHandle::released,
                this, &FlowchartShapeItem::resizeHandleReleased);
    }

    addConnectorHandle(ConnectionSide::Top);
    addConnectorHandle(ConnectionSide::Left);
    addConnectorHandle(ConnectionSide::Bottom);
    addConnectorHandle(ConnectionSide::Right);

    connectorItemManager_ = new ConnectorItemManager;
    connect(this, &FlowchartShapeItem::moved, connectorItemManager_,
            &ConnectorItemManager::updateConnectorItems);
    connect(this, &FlowchartShapeItem::resizedByHands, connectorItemManager_,
            &ConnectorItemManager::updateConnectorItems);
}

FlowchartShapeItem::~FlowchartShapeItem()
{
    qDebug() << "FlowchartShapeItem: Destructor";
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
    Q_UNUSED(widget)

    // draw dash rect behind shape
    QPen pen(Qt::blue, 0.5, Qt::PenStyle::DashLine);
    pen.setDashPattern({5, 5});
    painter->setPen(pen);
    painter->drawRect(contentRect());

    // draw shape
    painter->setPen(option->state & QStyle::State_Selected ? Qt::green : Qt::black);
    painter->setBrush(Qt::white);
    painter->drawPath(shape_);
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
    updateConnectorHandlesPositions();
}

QVariant FlowchartShapeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && Grid::enabled()) {
        return Grid::snapToGrid(value.toPointF());
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
    emit pressed(this);
}

void FlowchartShapeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ShapeItemBase::mouseReleaseEvent(event);
    emit released();
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

void FlowchartShapeItem::updateResizeHandlesPositions()
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

void FlowchartShapeItem::addConnectorHandle(ConnectionSide connectionSide)
{
    auto connectorHandle = new ConnectorHandle(connectionSide, this);
    connect(connectorHandle, &ConnectorHandle::pressed,
            this,            &FlowchartShapeItem::connectorHandlePressed);
    connectorHanldes_.append(connectorHandle);
}

void FlowchartShapeItem::updateConnectorHandlesPositions()
{
    const int margin = 20;
    const QRectF rect = contentRect();
    for (auto handle : connectorHanldes_) {
        switch (handle->connectionSide()) {
        case ConnectionSide::Top:
            handle->setPos(QPointF(rect.center().x(), rect.top() - margin));
            break;
        case ConnectionSide::Left:
            handle->setPos(QPointF(rect.left() - margin, rect.center().y()));
            break;
        case ConnectionSide::Bottom:
            handle->setPos(QPointF(rect.center().x(), rect.bottom() + margin));
            break;
        case ConnectionSide::Right:
            handle->setPos(QPointF(rect.right() + margin, rect.center().y()));
            break;
        }
    }
}

void FlowchartShapeItem::initByShape(const QPainterPath &shape)
{
    setShape(shape);
    resizer_->setCompareRect(contentRect());
    updateResizeHandlesPositions();
    updateConnectorHandlesPositions();
}

bool FlowchartShapeItem::textEditingEnabled() const
{
    return (textItem_->textInteractionFlags() == Qt::TextEditorInteraction);
}

FlowchartShapesTextItem *FlowchartShapeItem::textItem() const
{
    return textItem_;
}

void FlowchartShapeItem::addConnectorItem(ConnectorItem *connectorItem)
{
    connectorItemManager_->addConnectorItem(connectorItem);
}
