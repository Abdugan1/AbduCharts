#ifndef GUIDELINES_H
#define GUIDELINES_H

#include "shapeitembase.h"

class FlowchartShapeItem;

enum Orientation {Horizontal, Vertical};

class GuideLine : public ShapeItemBase
{
public:
    explicit GuideLine(QGraphicsItem* parent = nullptr);

    QRectF contentRect() const override;

    QLineF line() const;
    void setLine(const QLineF &line);

private:
    QLineF line_;
};


//------------------------------------------------
//                PositionLine
//------------------------------------------------


class PositionLine : public GuideLine
{
    enum {Outside = 30};
public:
    enum Mode
    {
        Left,
        Right,
        Top,
        Bottom,
        Center
    };

    static PositionLine* fromVertical(const FlowchartShapeItem* verticalBeginItem,
                                      const FlowchartShapeItem* verticalEndItem,
                                      Mode mode);

    static PositionLine* fromHorizontal(const FlowchartShapeItem* horizontalBeginItem,
                                        const FlowchartShapeItem* horizontalEndItem,
                                        Mode mode);

    PositionLine* reversed(bool reverse);

    ~PositionLine();

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    explicit PositionLine(const QLineF& line);

private:    
    static QPointF getPosByMode(const FlowchartShapeItem* item, Mode mode);
};


class SizeLine : public GuideLine
{
public:
    explicit SizeLine(const QRectF rect, Orientation orientation);
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    Orientation orientation_;
};

//------------------------------------------------
//                GuideLineCreator
//------------------------------------------------


class GuideLineCreator
{
public:
    // PositionLine
    static QList<PositionLine*> createLeftPositionLines(const FlowchartShapeItem* movedItem,
                                                        const QList<FlowchartShapeItem*> shapeItems);

    static QList<PositionLine*> createRightPositionLines(const FlowchartShapeItem* movedItem,
                                                         const QList<FlowchartShapeItem*> shapeItems);

    static QList<PositionLine*> createTopPositionLines(const FlowchartShapeItem* movedItem,
                                                       const QList<FlowchartShapeItem*> shapeItems);

    static QList<PositionLine*> createBottomPositionLines(const FlowchartShapeItem* movedItem,
                                                          const QList<FlowchartShapeItem*> shapeItems);

    static QList<PositionLine*> createCenterPositionLines(const FlowchartShapeItem* movedItem,
                                                          const QList<FlowchartShapeItem*> shapeItems);

    // SizeLine
    static QList<SizeLine*> createSizeLine(const FlowchartShapeItem* resizedItem,
                                           const QRectF& resizedItemOldRect,
                                           const QRectF& resizedItemCurrentRect,
                                           const QList<FlowchartShapeItem*> shapeItems);
};

class PositionLineCreator
{
public:
    static QList<PositionLine*> createPositionLines(const FlowchartShapeItem* movedItem,
                                                    const QList<FlowchartShapeItem*> shapeItems,
                                                    PositionLine::Mode mode);
private:
    static const FlowchartShapeItem* getLeftPosItem(const FlowchartShapeItem* item1,
                                                    const FlowchartShapeItem* item2);

    static const FlowchartShapeItem* getRightPosItem(const FlowchartShapeItem* item1,
                                                     const FlowchartShapeItem* item2);

    static const FlowchartShapeItem* getTopPosItem(const FlowchartShapeItem* item1,
                                                   const FlowchartShapeItem* item2);

    static const FlowchartShapeItem* getBottomPosItem(const FlowchartShapeItem* item1,
                                                      const FlowchartShapeItem* item2);

    static bool hasVerticalMatch(const FlowchartShapeItem* item1,
                                 const FlowchartShapeItem* item2,
                                 PositionLine::Mode mode);

    static bool hasHorizontalMatch(const FlowchartShapeItem* item1,
                                   const FlowchartShapeItem* item2,
                                   PositionLine::Mode mode);

    static bool hasPositionLine(const FlowchartShapeItem* verticalBeginItem,
                                const FlowchartShapeItem* verticalEndItem);

    static bool isReversedCase(const FlowchartShapeItem* endItem,
                               const FlowchartShapeItem* movedItem,
                               Orientation o);
};

class SizeLineCreator
{
public:
    static QList<SizeLine*> createSizeLine(const FlowchartShapeItem* resizedItem,
                                           const QRectF& resizedItemOldRect,
                                           const QRectF& resizedItemCurrentRect,
                                           const QList<FlowchartShapeItem*> shapeItems);
};

#endif // GUIDELINES_H












