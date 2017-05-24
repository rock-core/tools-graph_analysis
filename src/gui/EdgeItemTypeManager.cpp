#include "EdgeItemTypeManager.hpp"
#include "EdgeItemSimple.hpp"

#include <QGraphicsItem>
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace gui {

EdgeItemTypeManager::EdgeItemTypeManager()
{
    registerVisualization("default",dynamic_cast<EdgeItemBase*>(new EdgeItemSimple()));
}

void EdgeItemTypeManager::registerVisualization(const edge::Type& type, EdgeItemBase* graphicsItem)
{
    try {
        graphicsItemByType(type);
        // wtf? just throw?
        throw std::runtime_error("graph_analysis::gui::EdgeItemTypeManager::registerVisualization: type '" + type + "' is already registered");
    } catch(...)
    {
        mClassVisualizationMap[type] = graphicsItem;
    }
}

EdgeItemBase* EdgeItemTypeManager::graphicsItemByType(const edge::Type& type)
{
    ClassVisualizationMap::const_iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        return mClassVisualizationMap["default"];
    }
    return it->second;
}

EdgeItemBase* EdgeItemTypeManager::createItem(GraphWidget* graphWidget, const graph_analysis::Edge::Ptr& edge, QGraphicsItem* parent, const std::string& type)
{
    // conditionally returning a clone of the default when type is an empty string; using type in the types map otherwise
    std::string edgeType = type;
    if(type.empty())
    {
        edgeType = edge->getClassName();
    }
    return graphicsItemByType(edgeType)->createNewItem(graphWidget, edge, parent);
}

QStringList EdgeItemTypeManager::getSupportedTypes() const
{
    QStringList supportedTypes;
    ClassVisualizationMap::const_iterator cit = mClassVisualizationMap.begin();
    for(; cit != mClassVisualizationMap.end(); ++cit)
    {
        supportedTypes << QString(cit->first.c_str());
    }
    return supportedTypes;
}

} // end namespace gui
} // end namespace graph_analysis
