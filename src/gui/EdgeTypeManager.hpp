#ifndef GRAPH_ANALYSIS_EDGETYPEMANAGER_HPP
#define GRAPH_ANALYSIS_EDGETYPEMANAGER_HPP

#include <map>
#include <base/Singleton.hpp>
#include <graph_analysis/Edge.hpp>

#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>


namespace graph_analysis {
namespace gui {

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

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGETYPEMANAGER_HPP
