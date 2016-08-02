#include "EdgeItemTypeManager.hpp"

#include <QGraphicsItem>

#include "graphitem/edges/Simple.hpp"
#include "layeritem/edges/Simple.hpp"

namespace graph_analysis {
namespace gui {

EdgeItemTypeManager::EdgeItemTypeManager()
{
    registerVisualization("default",dynamic_cast<EdgeItem*>(new graphitem::edges::Simple()));
}

void EdgeItemTypeManager::registerVisualization(const edge::Type& type, EdgeItem* graphicsItem)
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

EdgeItem* EdgeItemTypeManager::graphicsItemByType(const edge::Type& type)
{
    ClassVisualizationMap::const_iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        return mClassVisualizationMap["default"];
    }
    return it->second;
}

EdgeItem* EdgeItemTypeManager::createItem(GraphWidget* graphWidget, QGraphicsItem* source, QGraphicsItem* target, graph_analysis::Edge::Ptr edge, const std::string& type)
{
    // conditionally returning a clone of the default when type is an empty string; using type in the types map otherwise
    std::string edgeType = type;
    if(type.empty())
    {
        edgeType = edge->getClassName();
    }
    return graphicsItemByType(edgeType)->createNewItem(graphWidget, source, target, edge);
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
