#include "shapeitemcreator.h"
#include "editor/items/flowchartshapeitems.h"

FlowchartShapeItem *ShapeItemCreator::createShape(int itemType)
{
    switch (itemType) {
    case TerminalShapeItem::Type:   return (new TerminalShapeItem);
    case ProcessShapeItem::Type:    return (new ProcessShapeItem);
    case DecisionShapeItem::Type:   return (new DecisionShapeItem);
    case InOutShapeItem::Type:      return (new InOutShapeItem);
    }

    Q_UNREACHABLE();
}
