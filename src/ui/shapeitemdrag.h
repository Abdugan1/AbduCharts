#ifndef SHAPEITEMDRAG_H
#define SHAPEITEMDRAG_H

#include <QMimeData>
#include <QDrag>

class ShapeItemMimeData : public QMimeData
{
    Q_OBJECT
public:
    const QString &figureType() const;
    void setFigureType(const QString &newFigureType);

    static QString mimeType();

private:
    QString figureType_;
};


//------------------------------------------------
//                Decision
//------------------------------------------------


class ShapeItemDrag : public QDrag
{
    Q_OBJECT
public:
    explicit ShapeItemDrag(QObject *dragSource);

    void setFigureType(const QString& figureType);
};

#endif // SHAPEITEMDRAG_H
