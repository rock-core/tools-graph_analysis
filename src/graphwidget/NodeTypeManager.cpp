#include "NodeTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include "graphitem/Simple.hpp"

namespace omviz {

NodeTypeManager::NodeTypeManager()
{
    mClassVisualizationMap = boost::assign::map_list_of
        ("default", new graphitem::Simple())
        ("graph_analysis::Vertex", new graphitem::Simple())
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
        throw std::runtime_error("omviz::NodeTypeManager::registerVisualization: type '" + type + "' is already registered");
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
        throw std::invalid_argument("omviz::NodeTypeManager::graphicsItemByType: type '" + type + "' is not registered");
    }
    return it->second;
}

NodeItem* NodeTypeManager::createItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex)
{
    return graphicsItemByType(vertex->getClassName())->createNewItem(graphWidget, vertex);
}

void NodeTypeManager::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, const node::Type& type)
{
    QGraphicsItem* item = graphicsItemByType(type);
    item->paint(painter, option);
}

QPainterPath NodeTypeManager::shape(const node::Type& type)
{
    QGraphicsItem* item = graphicsItemByType(type);
    return item->shape();
}

QRectF NodeTypeManager::boundingRect(const node::Type& type)
{
    QGraphicsItem* item = graphicsItemByType(type);
    return item->boundingRect();
}

}
