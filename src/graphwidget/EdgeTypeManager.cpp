#include "EdgeTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>
#include "graphitem/edges/Simple.hpp"

namespace omviz {

EdgeTypeManager::EdgeTypeManager()
{
    mClassVisualizationMap = boost::assign::map_list_of
        ("default", dynamic_cast<EdgeItem*>(new graphitem::edges::Simple()))
    ;
}

EdgeTypeManager::~EdgeTypeManager()
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.begin();
    for(; it != mClassVisualizationMap.end(); ++it)
    {
        delete it->second;
    }
}

void EdgeTypeManager::registerVisualization(const edge::Type& type, EdgeItem* graphicsItem)
{
    try {
        graphicsItemByType(type);
        throw std::runtime_error("omviz::EdgeTypeManager::registerVisualization: type '" + type + "' is already registered");
    } catch(...)
    {
        mClassVisualizationMap[type] = graphicsItem;
    }
}

EdgeItem* EdgeTypeManager::graphicsItemByType(const edge::Type& type)
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        //LOG_DEBUG_S << "omviz::EdgeTypeManager::graphicsItemByType: type '" + type + "' is not registered. Using default.";
        return mClassVisualizationMap["default"];
    }
    
    return it->second;
}

EdgeItem* EdgeTypeManager::createItem(NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge)
{
    return graphicsItemByType(edge->getClassName())->createNewItem(sourceNode, targetNode, edge);
}

}
