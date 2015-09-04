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

/**
 * \file EdgeTypeManager.hpp
 * \class EdgeTypeManager
 * \brief factory for EdgeItem-s: clones different types of EdgeItem instances (i.e. graphical edges)
 */
class EdgeTypeManager : public base::Singleton<EdgeTypeManager>
{
public:
    typedef std::map<edge::Type, EdgeItem*> ClassVisualizationMap;

private:
    /// registration map - stores the registered types, mapping them to the example edge instances (i.e. from which new ones to be forked on request)
    ClassVisualizationMap mClassVisualizationMap;
    /// internal method to query the instance map
    EdgeItem* graphicsItemByType(const edge::Type& type);

public:
    /// constructor
    EdgeTypeManager();
    /// destructor
    ~EdgeTypeManager();

    // Register visualization class
    // takes ownership of graphicsItem
    void registerVisualization(const edge::Type& type, EdgeItem* graphicsItem);

    /**
     * \brief clones a new graphical edge of a specified type
     * \param graphWidget the managing graph widget to assume parenthood of the to-be-created item
     * \param sourceNode edge source port node
     * \param targetNode edge target port node
     * \param edge underlying conceptual edge
     * \param type optional type parameter
     * \return smart pointer to the newly created graphical edge instance
     */
    EdgeItem* createItem(GraphWidget* graphWidget, NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge, const std::string& type = std::string());

    QStringList getSupportedTypes() const;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGETYPEMANAGER_HPP
