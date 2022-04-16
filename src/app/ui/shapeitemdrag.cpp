#include "ui/shapeitemdrag.h"

QString ShapeItemMimeData::mimeType()
{
    return "application/shape-item";
}

int ShapeItemMimeData::itemType() const
{
    return itemType_;
}

void ShapeItemMimeData::setItemType(int itemType)
{
    itemType_ = itemType;
    setData(mimeType(), QByteArray());
}


//------------------------------------------------
//                Decision
//------------------------------------------------


ShapeItemDrag::ShapeItemDrag(QObject *dragSource)
    : QDrag(dragSource)
{

}

void ShapeItemDrag::setItemType(int itemType)
{
    ShapeItemMimeData* mimeData = new ShapeItemMimeData;
    mimeData->setItemType(itemType);
    setMimeData(mimeData);
}
