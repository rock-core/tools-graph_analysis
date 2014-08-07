#ifndef OMVIZ_EDGETYPEMANAGER_HPP
#define OMVIZ_EDGETYPEMANAGER_HPP

#include <map>
#include <base/Singleton.hpp>
#include <graph_analysis/Edge.hpp>

#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>


namespace omviz {

namespace edge {
    typedef std::string Type;
}

class NodeItem;
class GraphWidget;
class EdgeItem;

class EdgeTypeManager : public base::Singleton<EdgeTypeManager>
{
public:
    typedef std::map<edge::Type, EdgeItem*> ClassVisualizationMap;

private:
    ClassVisualizationMap mClassVisualizationMap;
    EdgeItem* graphicsItemByType(const edge::Type& type);

public:
    EdgeTypeManager();
    ~EdgeTypeManager();

    // Register visualization class
    // takes ownership of graphicsItem
    void registerVisualization(const edge::Type& type, EdgeItem* graphicsItem);

    EdgeItem* createItem(NodeItem* sourceNode, NodeItem* destNode, graph_analysis::Edge::Ptr edge);
};

} // end namespace omviz
#endif // OMVIZ_EDGETYPEMANAGER_HPP
