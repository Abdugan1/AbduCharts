#ifndef ADDCONNECTORCOMMAND_H
#define ADDCONNECTORCOMMAND_H

#include "addcommand.h"

class AddConnectorCommand : public AddCommand
{
public:
    explicit AddConnectorCommand(ConnectorItem* connectorItem, Scene* scene);

    void undo() override;
    void redo() override;

private:
    ConnectorItem* connectorItem_ = nullptr;
};

#endif // ADDCONNECTORCOMMAND_H
