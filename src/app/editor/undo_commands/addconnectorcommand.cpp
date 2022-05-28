#include "addconnectorcommand.h"
#include "editor/connectoritem.h"
#include "editor/scene.h"

#include <QDebug>

AddConnectorCommand::AddConnectorCommand(ConnectorItem *connectorItem, Scene *scene)
    : connectorItem_(connectorItem)
{
    connectorItem_->setSelected(true);

    setScene(scene);
}

void AddConnectorCommand::undo()
{
    qDebug() << "AddConnectorCommand: undo";
    scene()->removeConnectorItem(connectorItem_);
}

void AddConnectorCommand::redo()
{
    qDebug() << "AddConnectorCommand: redo";
    scene()->addConnectorItem(connectorItem_);
}
