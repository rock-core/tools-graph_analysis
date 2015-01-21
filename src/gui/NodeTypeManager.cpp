#include "NodeTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>
#include "graphitem/Simple.hpp"
#include "graphitem/Resource.hpp"

namespace graph_analysis {
namespace gui {

NodeTypeManager::NodeTypeManager()
{
    mClassVisualizationMap = boost::assign::map_list_of
        ("default", dynamic_cast<NodeItem*>(new graphitem::Resource()))
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

void NodeTypeManager::registerVisualization(const node::Type& type, NodeItem* graphicsItem)
{
    try {
        graphicsItemByType(type);
        throw std::runtime_error("graph_analysis::gui::NodeTypeManager::registerVisualization: type '" + type + "' is already registered");
    } catch(...)
    {
        mClassVisualizationMap[type] = graphicsItem;
    }
}

NodeItem* NodeTypeManager::graphicsItemByType(const node::Type& type)
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        LOG_DEBUG_S << "graph_analysis::gui::NodeTypeManager::graphicsItemByType: type '" + type + "' is not registered. Using default.";
        return mClassVisualizationMap["default"];
    }

    LOG_DEBUG_S << "graph_analysis::gui::NodeTypeManager::graphicsItemByType: type '" + type + "' registered.";
    return it->second;
}

NodeItem* NodeTypeManager::createItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex)
{
    return graphicsItemByType(vertex->getClassName())->createNewItem(graphWidget, vertex);
}

} // end namespace gui
} // end namespace graph_analysis
