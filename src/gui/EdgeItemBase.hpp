#ifndef GRAPH_ANALYSIS_GUI_EDGEITEMBASE_HPP
#define GRAPH_ANALYSIS_GUI_EDGEITEMBASE_HPP

#include <QGraphicsItemGroup>
#include <graph_analysis/Edge.hpp>

#include "GraphicsItemTypes.hpp"

namespace graph_analysis
{
namespace gui
{

class GraphWidget;
class VertexItemBase;

/**
 * \file EdgeItemBase.hpp
 * \class EdgeItemBase
 * \brief graphical node representation interface
 * \details used as polymorphic base for several graphical node implementations
 */
class EdgeItemBase : public QGraphicsItemGroup
{
  public:
    /**
     * \brief constructor
     * \param graphWidget the parent and managing graph widget
     * \param edge the internal conceptual edge
     * \param parent the parent
     */
    EdgeItemBase(GraphWidget *graphWidget, graph_analysis::Edge::Ptr edge,
                 VertexItemBase *source, VertexItemBase *target,
                 QGraphicsItem *parent);

    /// destructor
    virtual ~EdgeItemBase(){};

    virtual int type() const { return EdgeItemBaseType; };

    virtual void adjust();

    /// getter method for retrieving the underlying conceptual graph edge
    graph_analysis::Edge::Ptr getEdge() const { return mpEdge; }
    /// setter method for updating the underlying conceptual graph edge
    void setEdge(graph_analysis::Edge::Ptr edge) { mpEdge = edge; }
    /// getter method for retrieving the parent managing graph widget
    GraphWidget *getGraphWidget() const { return mpGraphWidget; }

  protected:
    /// underlying graph edge pointer
    graph_analysis::Edge::Ptr mpEdge;

    /// parent managing graph widget
    GraphWidget *mpGraphWidget;

    VertexItemBase* mpSource;
    VertexItemBase* mpTarget;

    QPointF mSourcePoint;
    QPointF mTargetPoint;

    // unsure...
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

/* simplest possible implementation: just a line from source to target */
class EdgeItemSimple : public EdgeItemBase
{
  public:
    EdgeItemSimple(GraphWidget *graphWidget, graph_analysis::Edge::Ptr edge,
                   VertexItemBase *source, VertexItemBase *target,
                   QGraphicsItem *parent);
    ~EdgeItemSimple();
    virtual int type() const { return EdgeItemSimpleType; };

    void adjust();

  private:
    QPointF getIntersectionPoint(QGraphicsItem *item) const;

    int mArrowSize;
    QGraphicsTextItem *mpLabel;
    QGraphicsLineItem *mpLine;
    QGraphicsPolygonItem *mpArrowHead;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
