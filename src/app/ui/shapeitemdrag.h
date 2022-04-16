#ifndef SHAPEITEMDRAG_H
#define SHAPEITEMDRAG_H

#include <QMimeData>
#include <QDrag>

class ShapeItemMimeData : public QMimeData
{
    Q_OBJECT
public:
    static QString mimeType();

    int itemType() const;
    void setItemType(int itemType);

private:
    int itemType_ = -1;
};


//------------------------------------------------
//                Decision
//------------------------------------------------


class ShapeItemDrag : public QDrag
{
    Q_OBJECT
public:
    explicit ShapeItemDrag(QObject *dragSource);

    void setItemType(int itemType);
};

#endif // SHAPEITEMDRAG_H
