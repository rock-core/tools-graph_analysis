#ifndef OMVIZ_NODETYPEMANAGER_HPP
#define OMVIZ_NODETYPEMANAGER_HPP

#include <map>
#include <boost/assign/list_of.hpp>
#include <base/Singleton.hpp>
#include <graph_analysis/Vertex.hpp>

#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>


namespace omviz {

class GraphWidget;
class NodeItem;

namespace node {
    typedef std::string Type;
}

class NodeTypeManager;

class NodeTypeManager : public base::Singleton<NodeTypeManager>
{
public:
    typedef std::map<std::string, NodeItem*> ClassVisualizationMap;

private:
    ClassVisualizationMap mClassVisualizationMap;
    NodeItem* graphicsItemByType(const node::Type& type);

public:
    NodeTypeManager();
    ~NodeTypeManager();

    // Register visualization class
    // takes ownership of graphicsItem
    void registerVisualization(const node::Type& type, NodeItem* graphicsItem);

    NodeItem* createItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, const node::Type& type);
    QPainterPath shape(const node::Type& type);
    QRectF boundingRect(const node::Type& type);
};

} // end namespace omviz
#endif // OMVIZ_NODETYPEMANAGER_HPP
