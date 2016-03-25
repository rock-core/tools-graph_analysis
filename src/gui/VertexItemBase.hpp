#ifndef GRAPH_ANALYSIS_GUI_VERTEXITEMBASE_HPP
#define GRAPH_ANALYSIS_GUI_VERTEXITEMBASE_HPP

#include <QGraphicsItem>
#include <graph_analysis/Vertex.hpp>

#include "GraphicsItemTypes.hpp"

namespace graph_analysis
{

class BaseGraph;

namespace gui
{

class EdgeItemBase;
class GraphWidget;

/**
 * \file VertexItemBase.hpp
 * \class VertexItemBase
 * \brief graphical node representation interface
 * \details used as polymorphic base for several graphical node implementations
 */
class VertexItemBase : public QGraphicsItem
{
public:
    /**
     * \brief constructor
     * \param graphWidget the parent and managing graph widget
     * \param vertex the internal conceptual vertex
     * \param parent the parent
     */
    VertexItemBase(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex,
                   QGraphicsItem* parent);

    // destructor
    virtual ~VertexItemBase();

    virtual int type() const
    {
        return VertexItemBaseType;
    };

    /**
     * mechanism where edges can register as "to-be-notified" on position
     * change of this item
     */
    void registerConnection(EdgeItemBase* item);
    void deregisterConnection(EdgeItemBase* item);
    QVector<EdgeItemBase*> adjustConnections;

    /**
     * return a point where an edge should point to when it connects to this
     * vertex
     */
    virtual QPointF getConnector() const
    {
        return mapToScene(boundingRect().center());
    };

    /// getter method for retrieving the underlying conceptual graph vertex
    graph_analysis::Vertex::Ptr getVertex() const
    {
        return mpVertex;
    }
    /// setter method for updating the underlying conceptual graph vertex
    void setVertex(graph_analysis::Vertex::Ptr vertex)
    {
        mpVertex = vertex;
    }
    /// getter method for retrieving the parent managing graph widget
    GraphWidget* getGraphWidget() const
    {
        return mpGraphWidget;
    }

    /**
     * callback to trigger the base-graph to adjust all edges of this vertex
     * after its position has changed in the scene.
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);

protected:
    /// underlying graph vertex pointer
    graph_analysis::Vertex::Ptr mpVertex;

    /// parent managing graph widget. this should be reachable via "scene()"?
    GraphWidget* mpGraphWidget;

    /**
     * extracting the BaseGraph from the GraphWidget
     */
    shared_ptr<BaseGraph> getGraph() const;

    /**
     * this is used to update the statusbar of the current active widget with
     * information about the currently hovered (or not hovered) vertex
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
};

/**
 * simplest possible implementation: just a box with two strings -- type and
 * label
 */
class VertexItemSimple : public VertexItemBase
{
public:
    VertexItemSimple(GraphWidget* graphWidget,
                     graph_analysis::Vertex::Ptr vertex, QGraphicsItem* parent);
    ~VertexItemSimple();
    virtual int type() const
    {
        return VertexItemSimpleType;
    };

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = 0);
    QRectF boundingRect() const;

protected:
    /**
     * all items by default accept drag-n-drop events. override this function
     * in later classes to prevent certain drops?
     */
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event);
    void dropEvent(QGraphicsSceneDragDropEvent* event);

private:
    QGraphicsTextItem* mpLabel;
    QGraphicsTextItem* mpClassName;
    QGraphicsRectItem* mpRect;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
