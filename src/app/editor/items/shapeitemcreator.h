#ifndef SHAPEITEMCREATOR_H
#define SHAPEITEMCREATOR_H

class FlowchartShapeItem;
class QString;

class ShapeItemCreator
{
public:
    static FlowchartShapeItem* createShape(int itemType);
};

#endif // SHAPEITEMCREATOR_H
