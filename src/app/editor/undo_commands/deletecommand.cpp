#include "deletecommand.h"
#include "deleteshapecommand.h"
#include "deletetextcommand.h"
#include "deleteconnectorcommand.h"

DeleteCommand *DeleteCommand::fromShapeItem(FlowchartShapeItem *shapeItem, Scene *scene)
{
    return new DeleteShapeCommand(shapeItem, scene);
}

DeleteCommand *DeleteCommand::fromTextItem(FlowchartTextItem *textItem, Scene *scene)
{
    return new DeleteTextCommand(textItem, scene);
}

DeleteCommand *DeleteCommand::fromConnectorItem(ConnectorItem *connectorItem, Scene *scene)
{
    return new DeleteConnectorCommand(connectorItem, scene);
}

DeleteCommand::DeleteCommand(QUndoCommand *parent)
    : QUndoCommand(parent)
{

}

Scene *DeleteCommand::scene() const
{
    return scene_;
}

void DeleteCommand::setScene(Scene *newScene)
{
    scene_ = newScene;
}
