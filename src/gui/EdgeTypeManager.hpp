#ifndef GRAPH_ANALYSIS_EDGETYPEMANAGER_HPP
#define GRAPH_ANALYSIS_EDGETYPEMANAGER_HPP

#include <map>
#include <base/Singleton.hpp>
#include <graph_analysis/Edge.hpp>

#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QGraphicsView>


namespace graph_analysis {
namespace gui {

namespace edge {
    typedef std::string Type;
}

class NodeItem;
class EdgeItem;
class GraphWidget;

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

    EdgeItem* createItem(GraphWidget* graphWidget, NodeItem* sourceNode, int sourceNodePortID, NodeItem* targetNode, int targetNodePortID, graph_analysis::Edge::Ptr edge, const std::string& type = std::string());
    EdgeItem* createItem(GraphWidget* graphWidget, NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge, const std::string& type = std::string());
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGETYPEMANAGER_HPP
