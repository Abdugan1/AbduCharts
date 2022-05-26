#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class Scene;
class ConnectorItem;

class AddCommand : public QUndoCommand
{
public:
    static AddCommand* fromShapeItem(int itemType, const QPointF& pos, Scene* scene);
    static AddCommand* fromTextItem(const QString& text, const QPointF& pos, Scene* scene);
    static AddCommand* fromConnectorItem(ConnectorItem* connectorItem, Scene* scene);

protected:
    QPointF initialPos() const;
    void setInitialPos(QPointF newInitialPos);

    Scene *scene() const;
    void setScene(Scene *newScene);

    explicit AddCommand(QUndoCommand* parent = nullptr);

private:
    QPointF initialPos_;
    Scene* scene_ = nullptr;
};

#endif // ADDCOMMAND_H
