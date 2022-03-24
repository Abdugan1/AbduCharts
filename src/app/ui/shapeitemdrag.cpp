#include "ui/shapeitemdrag.h"

const QString &ShapeItemMimeData::figureType() const
{
    return figureType_;
}

void ShapeItemMimeData::setFigureType(const QString &newFigureType)
{
    figureType_ = newFigureType;
    setData(mimeType(), QByteArray());
}

QString ShapeItemMimeData::mimeType()
{
    return "application/shape-item";
}


//------------------------------------------------
//                Decision
//------------------------------------------------


ShapeItemDrag::ShapeItemDrag(QObject *dragSource)
    : QDrag(dragSource)
{

}

void ShapeItemDrag::setFigureType(const QString &figureType)
{
    ShapeItemMimeData* mimeData = new ShapeItemMimeData;
    mimeData->setFigureType(figureType);
    setMimeData(mimeData);
}
