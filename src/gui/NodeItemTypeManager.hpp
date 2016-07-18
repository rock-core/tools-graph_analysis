#ifndef GRAPH_ANALYSIS_GUI_NODE_ITEM_TYPE_MANAGER_HPP
#define GRAPH_ANALYSIS_GUI_NODE_ITEM_TYPE_MANAGER_HPP

#include <map>
#include <base-logging/Singleton.hpp>
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
 * \file NodeItemTypeManager.hpp
 * \class NodeItemTypeManager
 * \brief factory for NodeItem-s: clones different types of NodeItem instances (i.e. graphical nodes)
 */
class NodeItemTypeManager : public base::Singleton<NodeItemTypeManager>
{
public:
    typedef std::map<node::Type, NodeItem*> ClassVisualizationMap;

private:
    /// registration map - stores the registered types, mapping them to the example node instances (i.e. from which new ones to be forked on request)
    ClassVisualizationMap mClassVisualizationMap;
    /// internal method to query the instance map
    NodeItem* graphicsItemByType(const node::Type& type, bool throwOnDefault = false);

protected:
    /// constructor
    NodeItemTypeManager();
    /// destructor
    ~NodeItemTypeManager();
    friend class base::Singleton<NodeItemTypeManager>;
public:

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

    /**
     * Get list of supported visualizations
     * \return a list of supported node visualization types
     */
    QStringList getSupportedTypes() const;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_NODE_ITEM_TYPE_MANAGER_HPP
