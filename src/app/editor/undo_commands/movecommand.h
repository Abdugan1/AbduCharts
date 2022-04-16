#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class QGraphicsItem;

class MoveCommand : public QUndoCommand
{
public:
    explicit MoveCommand(QGraphicsItem* shapeItem, const QPointF& oldPos);

    void undo() override;
    void redo() override;

private:
    QGraphicsItem* item_ = nullptr;

    QPointF oldPos_;
    QPointF newPos_;
};

#endif // MOVECOMMAND_H
