#include "addcommand.h"
#include "addshapecommand.h"
#include "addtextcommand.h"
#include "addconnectorcommand.h"

#include <QDebug>

AddCommand *AddCommand::fromShapeItem(int itemType, const QPointF &pos, Scene *scene)
{
    return new AddShapeCommand(itemType, pos, scene);
}

AddCommand *AddCommand::fromTextItem(const QString &text, const QPointF &pos, Scene *scene)
{
    return new AddTextCommand(text, pos, scene);
}

AddCommand *AddCommand::fromConnectorItem(ConnectorItem *connectorItem, Scene *scene)
{
    return new AddConnectorCommand(connectorItem, scene);
}

AddCommand::AddCommand(QUndoCommand *parent)
    : QUndoCommand(parent)
{

}

Scene *AddCommand::scene() const
{
    return scene_;
}

void AddCommand::setScene(Scene *newScene)
{
    scene_ = newScene;
}

QPointF AddCommand::initialPos() const
{
    return initialPos_;
}

void AddCommand::setInitialPos(QPointF newInitialPos)
{
    initialPos_ = newInitialPos;
}
