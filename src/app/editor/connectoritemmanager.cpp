#include "editor/connectoritemmanager.h"
#include "editor/connectoritem.h"

#include <QDebug>

ConnectorItemManager::ConnectorItemManager(QObject *parent)
    : QObject(parent)
{

}

void ConnectorItemManager::addConnectorItem(ConnectorItem *connectorItem)
{
    if (connectorItem->startItem() != connectorItem->endItem())
        connectorItems_.push_back(connectorItem);
}

void ConnectorItemManager::updateConnectorItems()
{
    for (auto connectorItem : qAsConst(connectorItems_)) {
        if (connectorItem->scene())
            connectorItem->updateConnectionPath();
    }
}
