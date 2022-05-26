#include "addtextcommand.h"
#include "editor/items/textitem.h"
#include "editor/scene.h"

#include <QDebug>

AddTextCommand::AddTextCommand(const QString &text, const QPointF &initPos, Scene* scene)
{
    textItem_ = new TextItem;
    textItem_->setPlainText(text);
    textItem_->enableTextEditing();

    setInitialPos(initPos - textItem_->boundingRect().center());
    setScene(scene);
}

void AddTextCommand::undo()
{
    qDebug() << "AddTextCommand: undo";
    scene()->removeTextItem(textItem_);
}

void AddTextCommand::redo()
{
    qDebug() << "AddTextCommand: redo";
    textItem_->setPos(initialPos());
    scene()->addTextItem(textItem_);
}
