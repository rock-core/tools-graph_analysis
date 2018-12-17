#ifndef GRAPH_ANALYSIS_GUI_VERTEX_ITEM_TYPE_MANAGER_HPP
#define GRAPH_ANALYSIS_GUI_VERTEX_ITEM_TYPE_MANAGER_HPP

#include <map>
#include <base-logging/Singleton.hpp>
#include "../Vertex.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QGraphicsView>

namespace graph_analysis {
namespace gui {

namespace vertex {
    typedef std::string Type;
}

class VertexItemBase;
class GraphWidget;

/**
 * \file VertexItemTypeManager.hpp
 * \class VertexItemTypeManager
 * \brief factory for VertexItem-s: clones different types of VertexItem instances (i.e. graphical vertexs)
 */
class VertexItemTypeManager : public base::Singleton<VertexItemTypeManager>
{
public:
    typedef std::map<vertex::Type, VertexItemBase*> ClassVisualizationMap;

private:
    /// registration map - stores the registered types, mapping them to the example vertex instances (i.e. from which new ones to be forked on request)
    ClassVisualizationMap mClassVisualizationMap;
    /// internal method to query the instance map
    VertexItemBase* graphicsItemByType(const vertex::Type& type, bool throwOnDefault = false);

protected:
    /// constructor
    VertexItemTypeManager();
    /// destructor
    ~VertexItemTypeManager();
    friend class base::Singleton<VertexItemTypeManager>;
public:

    // Register visualization class
    // takes ownership of graphicsItem
    void registerVisualization(const vertex::Type& type, VertexItemBase* graphicsItem, bool throwOnAlreadyRegistered = false);

    /**
     * \brief clones a new graphical vertex of a specified type
     * \param graphWidget the managing graph widget to assume parenthood of the to-be-created item
     * \param vertex underlying conceptual graph vertex
     * \param parent Parent of this item
     * \param type optional preferred type
     * \return smart pointer to the newly created graphical vertex instance
     */
    VertexItemBase* createItem(GraphWidget* graphWidget,
            const graph_analysis::Vertex::Ptr& vertex,
            QGraphicsItem* parent,
            const std::string& type = std::string());

    /**
     * Get list of supported visualizations
     * \return a list of supported vertex visualization types
     */
    QStringList getSupportedTypes() const;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_VERTEX_ITEM_TYPE_MANAGER_HPP
