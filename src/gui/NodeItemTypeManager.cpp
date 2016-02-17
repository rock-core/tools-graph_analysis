#include "NodeItemTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>
#include "graphitem/Simple.hpp"
#include "graphitem/Cluster.hpp"
#include "layeritem/Resource.hpp"

namespace graph_analysis {
namespace gui {

NodeItemTypeManager::NodeItemTypeManager()
{
    registerVisualization("default", dynamic_cast<NodeItem*>(new graphitem::Cluster()));
    registerVisualization(layeritem::Resource::sType, dynamic_cast<NodeItem*>(new layeritem::Resource())); // layering graphical node instance
}

NodeItemTypeManager::~NodeItemTypeManager()
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.begin();
    for(; it != mClassVisualizationMap.end(); ++it)
    {
        delete it->second;
    }
}

void NodeItemTypeManager::registerVisualization(const node::Type& type, NodeItem* graphicsItem, bool throwOnAlreadyRegistered)
{
    try {
        graphicsItemByType(type, true);
    } catch(...)
    {
        mClassVisualizationMap[type] = graphicsItem;
    }
    LOG_WARN_S << "graph_analysis::gui::NodeItemTypeManager::registerVisualization: type '" + type + "' is already registered.";
    if(throwOnAlreadyRegistered)
    {
        throw std::runtime_error("graph_analysis::gui::NodeItemTypeManager::registerVisualization: type '" + type + "' is already registered");
    }
}

NodeItem* NodeItemTypeManager::graphicsItemByType(const node::Type& type, bool throwOnDefault)
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        LOG_DEBUG_S << "graph_analysis::gui::NodeItemTypeManager::graphicsItemByType: type '" + type + "' is not registered. Using default.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::gui::NodeItemTypeManager::graphicsItemByType: type '" + type + "' is not registered");
        }
        return mClassVisualizationMap["default"];
    }

    LOG_DEBUG_S << "graph_analysis::gui::NodeItemTypeManager::graphicsItemByType: type '" + type + "' registered.";
    return it->second;
}

NodeItem* NodeItemTypeManager::createItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex, const std::string& type)
{
    // conditionally returning a clone of the default when type is an empty string; using type in the types map otherwise
    return (type.empty() ? graphicsItemByType(vertex->getClassName()) : graphicsItemByType(type))->createNewItem(graphWidget, vertex);
}

QStringList NodeItemTypeManager::getSupportedTypes() const
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
