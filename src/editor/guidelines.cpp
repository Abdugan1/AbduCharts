#include "editor/guidelines.h"
#include "editor/items/flowchartshapeitem.h"

#include <QPainter>
#include <QDebug>
#include <QtMath>

using Mode = PositionLine::Mode;

const QPen GuideLinePen(Qt::darkCyan, 1.5);

QRectF getPosAddedRect(const FlowchartShapeItem* item)
{
    QRectF rect = item->shape().boundingRect();
    return QRectF(rect.x() + item->pos().x(),
                  rect.y() + item->pos().y(),
                  rect.width(), rect.height());
}

GuideLine::GuideLine(QGraphicsItem *parent)
    : ShapeItemBase(parent)
{
    setZValue(INFINITY);
}

QRectF GuideLine::contentRect() const
{
    return QRectF(line_.x1(),
                  line_.y1(),
                  line_.x2() - line_.x1(),
                  line_.y2() - line_.y1())
            .normalized();
}

QLineF GuideLine::line() const
{
    return line_;
}

void GuideLine::setLine(const QLineF &line)
{
    line_ = line;
}

PositionLine *PositionLine::fromHorizontal(const FlowchartShapeItem *horizontalBeginItem,
                                           const FlowchartShapeItem *horizontalEndItem,
                                           PositionLine::Mode mode)
{
    QRectF beginItemRect = horizontalBeginItem->shape().boundingRect();
    QRectF endItemRect   = horizontalEndItem->shape().boundingRect();
    QPointF begin = getPosByMode(horizontalBeginItem, mode);
    QPointF end   = getPosByMode(horizontalEndItem, mode);
    begin.setX(begin.x() - beginItemRect.width() / 2 - Outside);
    end.setX(end.x() + endItemRect.width() / 2 + Outside);
    return new PositionLine({begin, end});

}

PositionLine *PositionLine::fromVertical(const FlowchartShapeItem *verticalBeginItem,
                                         const FlowchartShapeItem *verticalEndItem,
                                         PositionLine::Mode mode)
{
    QRectF beginItemRect = verticalBeginItem->shape().boundingRect();
    QRectF endItemRect   = verticalEndItem->shape().boundingRect();
    QPointF begin = getPosByMode(verticalBeginItem, mode);
    QPointF end   = getPosByMode(verticalEndItem, mode);
    begin.setY(begin.y() - beginItemRect.height() / 2 - Outside);
    end.setY(end.y() + endItemRect.height() / 2 + Outside);
    return new PositionLine({begin, end});
}

PositionLine *PositionLine::reversed(bool reverse)
{
    if (reverse) {
        setLine({line().p2(), line().p1()});
    }
    return this;
}

PositionLine::~PositionLine()
{
    qDebug() << "PositionLine: Destructor";
}

//------------------------------------------------
//                PositionLine
//------------------------------------------------

QRectF PositionLine::boundingRect() const
{
    return contentRect() + margins();
}

void PositionLine::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(GuideLinePen);
    painter->drawLine(line());
}

PositionLine::PositionLine(const QLineF &line)
{
    setMargins(QMargins(5, 5, 5, 5));
    setLine(line);
}

QPointF PositionLine::getPosByMode(const FlowchartShapeItem *item, PositionLine::Mode mode)
{
    switch (mode) {
    case Left:   return {getPosAddedRect(item).left(),       getPosAddedRect(item).center().y()};
    case Right:  return {getPosAddedRect(item).right(),      getPosAddedRect(item).center().y()};
    case Top:    return {getPosAddedRect(item).center().x(), getPosAddedRect(item).top()};
    case Bottom: return {getPosAddedRect(item).center().x(), getPosAddedRect(item).bottom()};
    case Center: return item->pos();

    default: Q_UNREACHABLE(); break;
    }
}


//------------------------------------------------
//                SizeLine
//------------------------------------------------


SizeLine::SizeLine(const QRectF rect, Orientation orientation)
    : orientation_(orientation)
{
    const int Spacing = 20;

    if (orientation == Horizontal) {
        QPointF begin(rect.left(), rect.bottom() + Spacing);
        QPointF end(rect.right(),  rect.bottom() + Spacing);
        setLine({begin, end});
    } else {
        QPointF begin(rect.right() + Spacing, rect.top());
        QPointF end(rect.right()   + Spacing, rect.bottom());
        setLine({begin, end});
    }
}

QRectF SizeLine::boundingRect() const
{
    return contentRect() + margins();
}

void SizeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(GuideLinePen);
    painter->drawLine(line());

    const int SideLenght = 10;
    if (orientation_ == Orientation::Horizontal) {
        QLineF leftSide({line().p1().x(), line().p1().y() - SideLenght / 2},
                        {line().p1().x(), line().p1().y() + SideLenght / 2});

        QLineF rightSide({line().p2().x(), line().p2().y() - SideLenght / 2},
                         {line().p2().x(), line().p2().y() + SideLenght / 2});

        painter->drawLine(leftSide);
        painter->drawLine(rightSide);
    } else {
        QLineF topSide({line().p1().x() - SideLenght / 2, line().p1().y()},
                       {line().p1().x() + SideLenght / 2, line().p1().y()});

        QLineF bottomSide({line().p2().x() - SideLenght / 2, line().p2().y()},
                          {line().p2().x() + SideLenght / 2, line().p2().y()});

        painter->drawLine(topSide);
        painter->drawLine(bottomSide);
    }
}


//------------------------------------------------
//                GuideLineCreator
//------------------------------------------------


QList<PositionLine *> GuideLineCreator::createLeftPositionLines(const FlowchartShapeItem *movedItem,
                                                                const QList<FlowchartShapeItem *> shapeItems)
{
    return PositionLineCreator::createPositionLines(movedItem, shapeItems, PositionLine::Mode::Left);
}

QList<PositionLine *> GuideLineCreator::createRightPositionLines(const FlowchartShapeItem *movedItem,
                                                                 const QList<FlowchartShapeItem *> shapeItems)
{
    return PositionLineCreator::createPositionLines(movedItem, shapeItems, PositionLine::Mode::Right);
}

QList<PositionLine *> GuideLineCreator::createTopPositionLines(const FlowchartShapeItem *movedItem,
                                                               const QList<FlowchartShapeItem *> shapeItems)
{
    return PositionLineCreator::createPositionLines(movedItem, shapeItems, PositionLine::Mode::Top);
}

QList<PositionLine *> GuideLineCreator::createBottomPositionLines(const FlowchartShapeItem *movedItem,
                                                                  const QList<FlowchartShapeItem *> shapeItems)
{
    return PositionLineCreator::createPositionLines(movedItem, shapeItems, PositionLine::Mode::Bottom);
}

QList<PositionLine *> GuideLineCreator::createCenterPositionLines(const FlowchartShapeItem *movedItem,
                                                                  const QList<FlowchartShapeItem *> shapeItems)
{
    return PositionLineCreator::createPositionLines(movedItem, shapeItems, PositionLine::Mode::Center);
}

QList<SizeLine *> GuideLineCreator::createSizeLine(const FlowchartShapeItem *resizedItem,
                                                   const QRectF &resizedItemOldRect,
                                                   const QRectF &resizedItemCurrentRect,
                                                   const QList<FlowchartShapeItem *> shapeItems)
{    
    return SizeLineCreator::createSizeLine(resizedItem,
                                           resizedItemOldRect,
                                           resizedItemCurrentRect,
                                           shapeItems);
}


//------------------------------------------------
//                PositionLineCreator
//------------------------------------------------


QList<PositionLine *> PositionLineCreator::createPositionLines(const FlowchartShapeItem *movedItem,
                                                               const QList<FlowchartShapeItem *> shapeItems,
                                                               PositionLine::Mode mode)
{
    const FlowchartShapeItem* verticalBeginItem = movedItem;
    const FlowchartShapeItem* verticalEndItem   = movedItem;

    const FlowchartShapeItem* horizontalBeginItem = movedItem;
    const FlowchartShapeItem* horizontalEndItem   = movedItem;

    for (const auto& item : shapeItems) {
        if (hasVerticalMatch(item, movedItem, mode)) {
            verticalBeginItem = getTopPosItem(item, verticalBeginItem );
            verticalEndItem   = getBottomPosItem(item, verticalEndItem);
        }
        if (hasHorizontalMatch(item, movedItem, mode)) {
            horizontalBeginItem = getLeftPosItem(item, horizontalBeginItem);
            horizontalEndItem   = getRightPosItem(item, horizontalEndItem );
        }
    }

    QList<PositionLine*> positionLines;
    if (hasPositionLine(verticalBeginItem, verticalEndItem)) {
        Orientation orientation = Orientation::Vertical;
        positionLines.append(PositionLine::fromVertical(verticalBeginItem,
                                                        verticalEndItem,
                                                        mode)
                             ->reversed(isReversedCase(verticalEndItem, movedItem, orientation)));
    }
    if (hasPositionLine(horizontalBeginItem, horizontalEndItem)) {
        Orientation orientation = Orientation::Horizontal;
        positionLines.append(PositionLine::fromHorizontal(horizontalBeginItem,
                                                          horizontalEndItem,
                                                          mode)
                             ->reversed(isReversedCase(horizontalEndItem, movedItem, orientation)));
    }

    return positionLines;
}

const FlowchartShapeItem *PositionLineCreator::getLeftPosItem(const FlowchartShapeItem *item1,
                                                              const FlowchartShapeItem *item2)
{
    return qMin(item1->pos().x(), item2->pos().x()) == item1->pos().x() ? item1 : item2;
}

const FlowchartShapeItem *PositionLineCreator::getRightPosItem(const FlowchartShapeItem *item1,
                                                               const FlowchartShapeItem *item2)
{
    return qMax(item1->pos().x(), item2->pos().x()) == item1->pos().x() ? item1 : item2;
}

const FlowchartShapeItem *PositionLineCreator::getTopPosItem(const FlowchartShapeItem *item1,
                                                             const FlowchartShapeItem *item2)
{
    return qMin(item1->pos().y(), item2->pos().y()) == item1->pos().y() ? item1 : item2;
}

const FlowchartShapeItem *PositionLineCreator::getBottomPosItem(const FlowchartShapeItem *item1,
                                                                const FlowchartShapeItem *item2)
{
    return qMax(item1->pos().y(), item2->pos().y()) == item1->pos().y() ? item1 : item2;
}

bool PositionLineCreator::hasVerticalMatch(const FlowchartShapeItem *item1,
                                             const FlowchartShapeItem *item2,
                                             PositionLine::Mode mode)
{
    switch (mode) {
    case Mode::Left:   return getPosAddedRect(item1).left()  == getPosAddedRect(item2).left();
    case Mode::Right:  return getPosAddedRect(item1).right() == getPosAddedRect(item2).right();
    case Mode::Center: return item1->pos().x() == item2->pos().x();

    default: return false;
    }
}

bool PositionLineCreator::hasHorizontalMatch(const FlowchartShapeItem *item1,
                                               const FlowchartShapeItem *item2,
                                               PositionLine::Mode mode)
{
    switch (mode) {
    case Mode::Top:    return getPosAddedRect(item1).top()    == getPosAddedRect(item2).top();
    case Mode::Bottom: return getPosAddedRect(item1).bottom() == getPosAddedRect(item2).bottom();
    case Mode::Center: return item1->pos().y() == item2->pos().y();

    default: return false;
    }
}

bool PositionLineCreator::hasPositionLine(const FlowchartShapeItem *verticalBeginItem,
                                          const FlowchartShapeItem *verticalEndItem)
{
    return (verticalBeginItem != verticalEndItem);
}

bool PositionLineCreator::isReversedCase(const FlowchartShapeItem *endItem,
                                         const FlowchartShapeItem *movedItem,
                                         Orientation o)
{
    return o == Horizontal ? endItem->pos().x() != movedItem->pos().x()
                           : endItem->pos().y() != movedItem->pos().y();
}


QList<SizeLine *> SizeLineCreator::createSizeLine(const FlowchartShapeItem *resizedItem,
                                                  const QRectF &resizedItemOldRect,
                                                  const QRectF &resizedItemCurrentRect,
                                                  const QList<FlowchartShapeItem *> shapeItems)
{
    QList<SizeLine*> sizeLines;

    bool foundSameWidth  = false;
    bool foundSameHeight = false;
    const QRectF resizedRect = getPosAddedRect(resizedItem);
    for (const auto item : shapeItems) {
        QRectF itemRect = getPosAddedRect(item);
        if (resizedItemOldRect.width() != resizedItemCurrentRect.width()
                && itemRect.width() == resizedRect.width()
                && item != resizedItem) {
            sizeLines.append(new SizeLine(itemRect, Orientation::Horizontal));
            foundSameWidth = true;
        }
        if (resizedItemOldRect.height() != resizedItemCurrentRect.height()
                && itemRect.height() == resizedRect.height()
                && item != resizedItem) {
            sizeLines.append(new SizeLine(itemRect, Orientation::Vertical));
            foundSameHeight = true;
        }
    }

    if (foundSameWidth) {
        sizeLines.append(new SizeLine(resizedRect, Orientation::Horizontal));
    }
    if (foundSameHeight) {
        sizeLines.append(new SizeLine(resizedRect, Orientation::Vertical));
    }


    return sizeLines;
}
