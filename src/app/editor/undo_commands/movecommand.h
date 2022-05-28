#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class FlowchartShapeItem;
class FlowchartTextItem;

class MoveCommand : public QUndoCommand
{
public:
    static MoveCommand* fromShapeItem(FlowchartShapeItem* shapeItem, const QPointF& oldPos);
    static MoveCommand* fromTextItem(FlowchartTextItem* textItem, const QPointF& oldPos);

protected:
    QPointF oldPos() const;
    void setOldPos(QPointF newOldPos);

    QPointF newPos() const;
    void setNewPos(QPointF newNewPos);

    explicit MoveCommand(QUndoCommand* parent = nullptr);

private:
    QPointF oldPos_;
    QPointF newPos_;
};

#endif // MOVECOMMAND_H
