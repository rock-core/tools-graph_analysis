#ifndef GRAPH_ANALYSIS_GUI_EDGEITEMBASE_HPP
#define GRAPH_ANALYSIS_GUI_EDGEITEMBASE_HPP

#include <QGraphicsItem>
#include <graph_analysis/Edge.hpp>

#include "GraphicsItemTypes.hpp"

namespace graph_analysis
{
namespace gui
{

class GraphWidget;
class VertexItemBase;

/**
 * @file EdgeItemBase.hpp
 * @class EdgeItemBase
 * @brief standard implementation of graphical nodes for an Edge
 *
 *
 *
 * @details used as polymorphic base for several graphical node implementations
 */
class EdgeItemBase : public QGraphicsItem
{
public:
    /**
     * @brief constructor
     * @param graphWidget the parent and managing graph widget
     * @param edge the internal conceptual edge
     * @param parent the parent
     */
    EdgeItemBase(GraphWidget* graphWidget, graph_analysis::Edge::Ptr edge,
                 VertexItemBase* source, VertexItemBase* target,
                 QGraphicsItem* parent);

    virtual ~EdgeItemBase();
    virtual int type() const;

    /**
     * triggers this item to update its own position on the canvas
     *
     * this item can be coupled via the "registerPositionAdjustmentConnection"
     * to the position-change-signal of VertexItems.
     *
     * this function is to be called after position updates of the coupled
     * Vertex.
     *
     * does nothing by default.
     */
    virtual void adjustEdgePositioning();

    /** getter method for retrieving the underlying conceptual graph edge */
    graph_analysis::Edge::Ptr getEdge() const
    {
        return mpEdge;
    }
    /** setter method for updating the underlying conceptual graph edge */
    void setEdge(graph_analysis::Edge::Ptr edge)
    {
        mpEdge = edge;
    }
    /** getter method for retrieving the parent managing graph widget */
    GraphWidget* getGraphWidget() const
    {
        return mpGraphWidget;
    }

protected:
    /** underlying graph edge pointer */
    graph_analysis::Edge::Ptr mpEdge;

    /** parent managing graph widget */
    GraphWidget* mpGraphWidget;

    /**
     * the two source- and target-items, where this Edge is connected to
     */
    VertexItemBase* mpSourceItem;
    VertexItemBase* mpTargetItem;

    /** provide mouse-over status updates of the currently selected Edge */
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
};

/**
 *
 * simplest possible implementation: just a line from source to target
 *
 */
class EdgeItemSimple : public EdgeItemBase
{
public:
    EdgeItemSimple(GraphWidget* graphWidget, graph_analysis::Edge::Ptr edge,
                   VertexItemBase* source, VertexItemBase* target,
                   QGraphicsItem* parent);
    ~EdgeItemSimple();
    virtual int type() const;
    void adjustEdgePositioning();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;

    /**
     * two points of the source and target, where this edge should attach
     *
     * TODO: change this interface into "update according a list of points",
     * and move the intersection code in the base-widget as one very simple
     * layouter.
     */
    QPointF mSourcePoint;
    QPointF mTargetPoint;

private:
    QPointF getIntersectionPoint(QGraphicsItem* item) const;

    int mArrowSize;
    QGraphicsTextItem* mpLabel;
    QGraphicsTextItem* mpClassName;
    QGraphicsLineItem* mpLine;
    QGraphicsPolygonItem* mpArrowHead;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
