#ifndef GRAPH_ANALYSIS_EDGE_ITEM_TYPE_MANAGER_HPP
#define GRAPH_ANALYSIS_EDGE_ITEM_TYPE_MANAGER_HPP

#include <base-logging/Singleton.hpp>
#include "../Edge.hpp"
#include <QStringList>
#include <map>

class QGraphicsItem;

namespace graph_analysis {
namespace gui {

namespace edge {
    typedef std::string Type;
}

class VertexItemBase;
class EdgeItemBase;
class GraphWidget;

/**
 * \file EdgeItemTypeManager.hpp
 * \class EdgeItemTypeManager
 * \brief factory for EdgeItem-s: clones different types of EdgeItem instances (i.e. graphical edges)
 */
class EdgeItemTypeManager : public base::Singleton<EdgeItemTypeManager>
{
public:
    typedef std::map<edge::Type, EdgeItemBase*> ClassVisualizationMap;

private:
    /// registration map - stores the registered types, mapping them to the example edge instances (i.e. from which new ones to be forked on request)
    ClassVisualizationMap mClassVisualizationMap;
    /// internal method to query the instance map
    EdgeItemBase* graphicsItemByType(const edge::Type& type);

protected:
    /// constructor
    EdgeItemTypeManager();
    friend class base::Singleton<EdgeItemTypeManager>;
public:

    // Register visualization class
    // takes ownership of graphicsItem
    void registerVisualization(const edge::Type& type, EdgeItemBase* graphicsItem);

    /**
     * \brief clones a new graphical edge of a specified type
     * \param graphWidget the managing graph widget to assume parenthood of the to-be-created item
     * \param edge underlying conceptual edge
     * \param parent Parent of this item
     * \param type optional type parameter
     * \return smart pointer to the newly created graphical edge instance
     */
    EdgeItemBase* createItem(GraphWidget* graphWidget,
            const graph_analysis::Edge::Ptr& edge,
            QGraphicsItem* parent,
            const std::string& type = std::string());

    QStringList getSupportedTypes() const;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_ITEM_TYPE_MANAGER_HPP
