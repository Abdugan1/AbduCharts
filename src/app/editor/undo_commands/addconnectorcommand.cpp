#include "addconnectorcommand.h"
#include "editor/scene.h"

#include <QDebug>

AddConnectorCommand::AddConnectorCommand(ConnectorItem *connectorItem, Scene *scene)
    : connectorItem_(connectorItem)
{
    setScene(scene);
}

void AddConnectorCommand::undo()
{
    qDebug() << "AddConnectorCommand: undo";
    scene()->addConnectorItem(connectorItem_);
}

void AddConnectorCommand::redo()
{
    qDebug() << "AddConnectorCommand: redo";
    scene()->addConnectorItem(connectorItem_);
}
