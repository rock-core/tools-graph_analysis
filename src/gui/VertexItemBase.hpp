#ifndef GRAPH_ANALYSIS_GUI_VERTEXITEMBASE_HPP
#define GRAPH_ANALYSIS_GUI_VERTEXITEMBASE_HPP

#include <QGraphicsItemGroup>
#include <graph_analysis/Vertex.hpp>

#include "GraphicsItemTypes.hpp"

namespace graph_analysis
{
namespace gui
{

class GraphWidget;

/**
 * \file VertexItemBase.hpp
 * \class VertexItemBase
 * \brief graphical node representation interface
 * \details used as polymorphic base for several graphical node implementations
 */
class VertexItemBase : public QGraphicsItemGroup
{
  protected:
    /**
     * \brief constructor
     * \param graphWidget the parent and managing graph widget
     * \param vertex the internal conceptual vertex
     * \param parent the parent
     */
    VertexItemBase(GraphWidget *graphWidget, graph_analysis::Vertex::Ptr vertex,
                   QGraphicsItem *parent);

  public:
    /// empty constructor
    VertexItemBase() {}

    /// destructor
    virtual ~VertexItemBase(){};

    virtual int type() const { return VertexItemBaseType; };

    /// getter method for retrieving the underlying conceptual graph vertex
    graph_analysis::Vertex::Ptr getVertex() const { return mpVertex; }
    /// setter method for updating the underlying conceptual graph vertex
    void setVertex(graph_analysis::Vertex::Ptr vertex) { mpVertex = vertex; }
    /// getter method for retrieving the parent managing graph widget
    GraphWidget *getGraphWidget() const { return mpGraphWidget; }

  protected:
    /// underlying graph vertex pointer
    graph_analysis::Vertex::Ptr mpVertex;

    /// parent managing graph widget
    GraphWidget *mpGraphWidget;

    // unsure...
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

};

} // end namespace gui
} // end namespace graph_analysis
#endif
