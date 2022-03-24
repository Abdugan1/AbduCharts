#include "shapeitemcreator.h"
#include "editor/items/flowchartshapeitems.h"

FlowchartShapeItem* ShapeItemCreator::createShape(const QString &shapeType)
{
    if      (shapeType == "Terminal")      return (new TerminalShapeItem);
    else if (shapeType == "Process")       return (new ProcessShapeItem);
    else if (shapeType == "Decision")      return (new DecisionShapeItem);
    else if (shapeType == "Input/Output")  return (new InOutShapeItem);

    Q_UNREACHABLE();
    return nullptr;
}
