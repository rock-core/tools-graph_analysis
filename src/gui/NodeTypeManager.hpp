#ifndef GRAPH_ANALYSIS_GUI_NODETYPEMANAGER_HPP
#define GRAPH_ANALYSIS_GUI_NODETYPEMANAGER_HPP

#include <map>
#include <base/Singleton.hpp>
#include <graph_analysis/Vertex.hpp>

#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QGraphicsView>

namespace graph_analysis {
namespace gui {

namespace node {
    typedef std::string Type;
}

class NodeItem;
class GraphWidget;

/**
 * \class NodeTypeManager
 * \file NodeTypeManager.hpp
 * \brief factory for NodeItem-s: clones different types of NodeItem instances (i.e. graphical nodes)
 */
class NodeTypeManager : public base::Singleton<NodeTypeManager>
{
public:
    typedef std::map<node::Type, NodeItem*> ClassVisualizationMap;

private:
    /// registration map - stores the registered types, mapping them to the example node instances (i.e. from which new ones to be forked on request)
    ClassVisualizationMap mClassVisualizationMap;
    /// internal method to query the instance map
    NodeItem* graphicsItemByType(const node::Type& type, bool throwOnDefault = false);

public:
    /// constructor
    NodeTypeManager();
    /// destructor
    ~NodeTypeManager();

    // Register visualization class
    // takes ownership of graphicsItem
    void registerVisualization(const node::Type& type, NodeItem* graphicsItem, bool throwOnAlreadyRegistered = false);
    /**
     * \brief clones a new graphical node of a specified type
     * \param graphWidget the managing graph widget to assume parenthood of the to-be-created item
     * \param vertex underlying conceptual graph vertex
     * \param type optional preferred type
     * \return smart pointer to the newly created graphical node instance
     */
    NodeItem* createItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex, const std::string& type = std::string());
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_NODETYPEMANAGER_HPP
