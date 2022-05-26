#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>

class FlowchartShapeItem;
class FlowchartTextItem;
class ConnectorItem;
class Scene;

class DeleteCommand : public QUndoCommand
{
public:
    static DeleteCommand* fromShapeItem(FlowchartShapeItem* shapeItem, Scene* scene);
    static DeleteCommand* fromTextItem(FlowchartTextItem* textItem, Scene* scene);
    static DeleteCommand* fromConnectorItem(ConnectorItem* connectorItem, Scene* scene);

protected:
    Scene *scene() const;
    void setScene(Scene *newScene);

    explicit DeleteCommand(QUndoCommand* parent = nullptr);

private:
    Scene* scene_ = nullptr;
};

#endif // DELETECOMMAND_H
