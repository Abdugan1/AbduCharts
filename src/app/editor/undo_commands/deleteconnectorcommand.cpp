#include "deleteconnectorcommand.h"
#include "editor/scene.h"

#include <QDebug>

DeleteConnectorCommand::DeleteConnectorCommand(ConnectorItem *connectorItem, Scene *scene)
    : connectorItem_(connectorItem)
{
    setScene(scene);
}

void DeleteConnectorCommand::undo()
{
    qDebug() << "DeleteConnectorCommand: undo";
    scene()->addConnectorItem(connectorItem_);
}

void DeleteConnectorCommand::redo()
{
    qDebug() << "DeleteConnectorCommand: redo";
    scene()->removeConnectorItem(connectorItem_);
}
