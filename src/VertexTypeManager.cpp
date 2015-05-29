#include "VertexTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>
#include "graphitem/Simple.hpp"
#include "graphitem/Resource.hpp"

namespace graph_analysis {
namespace gui {

VertexTypeManager::VertexTypeManager()
{
    mClassVisualizationMap = boost::assign::map_list_of
        ("default", dynamic_cast<NodeItem*>(new graphitem::Resource()))
        // Change behaviour and appearance of edge if needed
        // ("graph_analysis::Vertex", dynamic_cast<NodeItem*>(new graphitem::Simple()))
    ;
}

VertexTypeManager::~VertexTypeManager()
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.begin();
    for(; it != mClassVisualizationMap.end(); ++it)
    {
        delete it->second;
    }
}

void VertexTypeManager::registerVisualization(const node::Type& type, NodeItem* graphicsItem)
{
    try {
        vertexByType(type);
        throw std::runtime_error("graph_analysis::gui::VertexTypeManager::registerVisualization: type '" + type + "' is already registered");
    } catch(...)
    {
        mClassVisualizationMap[type] = graphicsItem;
    }
}

Vertex::Ptr VertexTypeManager::vertexByType(const vertex::Type& type)
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        LOG_DEBUG_S << "graph_analysis::VertexTypeManager::graphicsItemByType: type '" + type + "' is not registered. Using default.";
        return mClassVisualizationMap["default"];
    }

    LOG_DEBUG_S << "graph_analysis::gui::VertexTypeManager::graphicsItemByType: type '" + type + "' registered.";
    return it->second;
}

Vertex::Ptr VertexTypeManager::createVertex(vertex::Type)
{
    return boost::make_shared<Vertex>(asdfasdf); // (new vertexByType(vertex::Type));
}

} // end namespace gui
} // end namespace graph_analysis
