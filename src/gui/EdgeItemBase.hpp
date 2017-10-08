#ifndef GRAPH_ANALYSIS_GUI_EDGEITEMBASE_HPP
#define GRAPH_ANALYSIS_GUI_EDGEITEMBASE_HPP

#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <graph_analysis/Edge.hpp>

#include "GraphicsItemTypes.hpp"

namespace graph_analysis {
namespace gui {

class GraphWidget;
class VertexItemBase;
class EdgeItemTypeManager;

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
    friend class EdgeItemTypeManager;

protected:
    EdgeItemBase();

public:
    /**
     * @brief constructor
     * @param graphWidget the parent and managing graph widget
     * @param edge the internal conceptual edge
     * @param parent the parent
     */
    EdgeItemBase(GraphWidget* graphWidget,
            const graph_analysis::Edge::Ptr& edge,
            QGraphicsItem* parent);

    virtual ~EdgeItemBase();
    virtual int type() const;

    virtual void adjustEdgePoints(QVector<QPointF> points);
    virtual void adjustEdgePositioning();

    /**
     * Calling this function in adjustEdgePositioning in a subclass
     * will render straight paths between vertices
     */
    void drawStraightEdge();

    /**
     * Calling this function in adjustEdgePositioning in a subclass
     * will render bezier curves between vertices
     */
    void drawBezierEdge();

    /**
     * Calling this function in adjustEdgePositioning in a subclass
     * will render an arrow head at the tip
     * needs to be called after(!) drawStraightEdge or drawBezierEdge
     */
    void drawArrowHead(size_t arrowSize);

    /**
     * two points of the source and target, where this edge should attach
     *
     * this can be as simple as two points which shall be connected by a
     * straight line.
     */
    QVector<QPointF> mPoints;

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

    /**
     * Compute the shape of the edge (based on the edge path and the arrowhead)
     */
    virtual QPainterPath shape() const;

    /**
     * Get the edge path, e.g., to allow setting the brush
     */
    QGraphicsPathItem* getEdgePath() const { return mpEdgePath; }

    /**
     * Get the arrow head, e.g., to allow setting the brush
     */
    QGraphicsPolygonItem* getArrowHead() const { return mpArrowHead; }

protected:
    /** underlying graph edge pointer */
    graph_analysis::Edge::Ptr mpEdge;

    /** parent managing graph widget */
    GraphWidget* mpGraphWidget;

    /** path for rendering an edge **/
    QGraphicsPathItem* mpEdgePath;
    /** render an arrow head **/
    QGraphicsPolygonItem* mpArrowHead;


    /** provide mouse-over status updates of the currently selected Edge */
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

    virtual EdgeItemBase* createNewItem(GraphWidget* graphWidget,
                const graph_analysis::Edge::Ptr& edge,
                QGraphicsItem* parent) const = 0;

};

} // end namespace gui
} // end namespace graph_analysis
#endif
