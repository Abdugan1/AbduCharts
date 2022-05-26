#ifndef DELETECONNECTORCOMMAND_H
#define DELETECONNECTORCOMMAND_H

#include "deletecommand.h"

class DeleteConnectorCommand : public DeleteCommand
{
public:
    explicit DeleteConnectorCommand(ConnectorItem* connectorItem, Scene* scene);

    void undo() override;
    void redo() override;

private:
    ConnectorItem* connectorItem_ = nullptr;
};

#endif // DELETECONNECTORCOMMAND_H
