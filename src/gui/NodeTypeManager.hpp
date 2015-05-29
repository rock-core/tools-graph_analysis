#ifndef GRAPH_ANALYSIS_GUI_NODETYPEMANAGER_HPP
#define GRAPH_ANALYSIS_GUI_NODETYPEMANAGER_HPP

#include <map>
#include <base/Singleton.hpp>
#include <graph_analysis/Vertex.hpp>

#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>


namespace graph_analysis {
namespace gui {

namespace node {
    typedef std::string Type;
}

class GraphWidget;
class NodeItem;

class NodeTypeManager : public base::Singleton<NodeTypeManager>
{
public:
    typedef std::map<node::Type, NodeItem*> ClassVisualizationMap;

private:
    ClassVisualizationMap mClassVisualizationMap;
    NodeItem* graphicsItemByType(const node::Type& type, bool throwOnDefault = false);

public:
    NodeTypeManager();
    ~NodeTypeManager();

    // Register visualization class
    // takes ownership of graphicsItem
    void registerVisualization(const node::Type& type, NodeItem* graphicsItem, bool throwOnAlreadyRegistered = false);
    NodeItem* createItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_NODETYPEMANAGER_HPP
