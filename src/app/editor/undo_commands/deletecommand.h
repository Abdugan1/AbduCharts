#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>

class FlowchartShapeItem;
class FlowchartTextItem;
class Scene;

class DeleteCommand : public QUndoCommand
{
public:
    static DeleteCommand* fromShapeItem(FlowchartShapeItem* shapeItem, Scene* scene);
    static DeleteCommand* fromTextItem(FlowchartTextItem* textItem, Scene* scene);

    void undo() override;
    void redo() override;

private:
    DeleteCommand() {}
private:
    FlowchartShapeItem* shapeItem_ = nullptr;
    FlowchartTextItem*  textItem_  = nullptr;

    Scene* scene_ = nullptr;
};

#endif // DELETECOMMAND_H
