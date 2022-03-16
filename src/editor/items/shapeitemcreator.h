#ifndef SHAPEITEMCREATOR_H
#define SHAPEITEMCREATOR_H

class FlowchartShapeItem;
class QString;

class ShapeItemCreator
{
public:
    static FlowchartShapeItem* createShape(const QString& shapeType);
};

#endif // SHAPEITEMCREATOR_H
