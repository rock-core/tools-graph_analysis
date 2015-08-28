#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_EDGES_SIMPLE_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_EDGES_SIMPLE_HPP

#include <graph_analysis/gui/EdgeItem.hpp>
#include <QPainterPath>
#include <QColor>
#include <QGraphicsLineItem>

/// string under which to register this edge item implementation in the factory
#define LAYER_EDGE_TYPE "LayerEdge"

namespace graph_analysis {
namespace gui {
namespace layeritem {
namespace edges {

class EdgeLabel;

/**
 * \file Simple.hpp
 * \class Simple
 * \brief graphical edge implementation
 * \details specific to layer widget graphical implementation: straight direct edge
 *      holding a text label on top of its middle region
 */
class Simple : public graph_analysis::gui::EdgeItem
{
private:
    /**
     * \brief constructor; private since instances of this class only spawn via the EdgeTypeManager factory class
     * \param graphWidget managing widget
     * \param sourceNode source vertex
     * \param targetNode target vertex
     * \param edge internal conceptual edge
     */
    Simple(GraphWidget* graphWidget, NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge);
public:
    /// empty constructor
    Simple() {}
    /// getter method for the graphical text label member field
    virtual EdgeLabel* getLabel() { return mpLabel; }
    /// destructor
    virtual ~Simple() {}

    /// adjusts edge segment length and starting and ending points
    virtual void adjust();
    /// adjusts graphical text label position
    void adjustLabel();

protected:
    /// retrieves the bounding rectangular box around the area occupied by the node in the scene
    virtual QRectF boundingRect() const;
    /// retrieves the path of the bounding rectangular box around the area occupied by the node in the scene
    virtual QPainterPath shape() const { return ::graph_analysis::gui::EdgeItem::shape(); }
    /// qt edge painting method - here the edge components get placed in the scene (the line segment and arrow tip)
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);
    /// qt hovering ENTER callback
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    /// qt hovering LEAVE callback
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    /// establishes where the edge line segment shall end at one of the extremities (computes intersection with the rectangular boundary of one of the endpoints)
    QPointF getIntersectionPoint(NodeItem* item, const QLineF& line);

    // edge cloning/spawning method used by the factory to produce new edges
    virtual EdgeItem* createNewItem(GraphWidget* graphWidget,
            NodeItem* sourceNode,
            NodeItem* targetNode,
            graph_analysis::Edge::Ptr edge) const
    { return new Simple(graphWidget, sourceNode, targetNode, edge); }

    virtual EdgeItem* createNewItem(GraphWidget* graphWidget,
            NodeItem* sourceNode,
            NodeItem::id_t sourceNodePortID,
            NodeItem* targetNode,
            NodeItem::id_t targetNodePortID,
            graph_analysis::Edge::Ptr edge) const
    { throw std::runtime_error("graph_analysis::gui::EdgeItem::createNewItem (@6 args) is not reimplemented"); }

    /// the graphical text label
    EdgeLabel* mpLabel;
    /// current qt drawing pen
    QPen mPen;
    /// the default qt drawing pen
    QPen mPenDefault;
    /// ending arrow tip
    QPolygonF mArrowHead;
    /// edge graphical line segment
    QLineF mLine;
};

} // end namespace edges
} // end namespace layeritem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_EDGES_SIMPLE_HPP
