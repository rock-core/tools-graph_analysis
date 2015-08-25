#include "NodeTypeManager.hpp"

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

NodeTypeManager::NodeTypeManager()
{
    mClassVisualizationMap = boost::assign::map_list_of
        ("default", dynamic_cast<NodeItem*>(new graphitem::Cluster()))
        (LAYER_NODE_TYPE, dynamic_cast<NodeItem*>(new layeritem::Resource())) // layering graphical node instance
        // Change behaviour and appearance of edge if needed
        // ("graph_analysis::Vertex", dynamic_cast<NodeItem*>(new graphitem::Simple()))
    ;
}

NodeTypeManager::~NodeTypeManager()
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.begin();
    for(; it != mClassVisualizationMap.end(); ++it)
    {
        delete it->second;
    }
}

void NodeTypeManager::registerVisualization(const node::Type& type, NodeItem* graphicsItem, bool throwOnAlreadyRegistered)
{
    try {
        graphicsItemByType(type, true);
    } catch(...)
    {
        mClassVisualizationMap[type] = graphicsItem;
    }
    LOG_WARN_S << "graph_analysis::gui::NodeTypeManager::registerVisualization: type '" + type + "' is already registered.";
    if(throwOnAlreadyRegistered)
    {
        throw std::runtime_error("graph_analysis::gui::NodeTypeManager::registerVisualization: type '" + type + "' is already registered");
    }
}

NodeItem* NodeTypeManager::graphicsItemByType(const node::Type& type, bool throwOnDefault)
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        LOG_DEBUG_S << "graph_analysis::gui::NodeTypeManager::graphicsItemByType: type '" + type + "' is not registered. Using default.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::gui::NodeTypeManager::graphicsItemByType: type '" + type + "' is not registered");
        }
        return mClassVisualizationMap["default"];
    }

    LOG_DEBUG_S << "graph_analysis::gui::NodeTypeManager::graphicsItemByType: type '" + type + "' registered.";
    return it->second;
}

NodeItem* NodeTypeManager::createItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex, const std::string& type)
{
    // conditionally returning a clone of the default when type is an empty string; using type in the types map otherwise
    return (type.empty() ? graphicsItemByType(vertex->getClassName()) : graphicsItemByType(type))->createNewItem(graphWidget, vertex);
}

} // end namespace gui
} // end namespace graph_analysis
