#ifndef CONNECTORITEMMANAGER_H
#define CONNECTORITEMMANAGER_H

#include <QObject>
#include <QList>

class ConnectorItem;

class ConnectorItemManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectorItemManager(QObject* parent = nullptr);

    void addConnectorItem(ConnectorItem* connectorItem);

public slots:
    void updateConnectorItems();

private:
    QList<ConnectorItem*> connectorItems_;
};

#endif // CONNECTORITEMMANAGER_H
