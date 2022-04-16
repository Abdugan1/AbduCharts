#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class FlowchartShapeItem;
class FlowchartTextItem;
class Scene;

class AddCommand : public QUndoCommand
{
public:
    static AddCommand* fromShapeItem(int itemType, const QPointF& pos, Scene* scene);
    static AddCommand* fromTextItem(const QString& text, const QPointF& pos, Scene* scene);

    void undo() override;
    void redo() override;

private:
    AddCommand() {};

private:
    FlowchartShapeItem* shapeItem_ = nullptr;
    FlowchartTextItem*  textItem_  = nullptr;

    QPointF initialPosition_;

    Scene* scene_ = nullptr;
};

#endif // ADDCOMMAND_H
