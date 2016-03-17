#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_EDGES_SIMPLE_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_EDGES_SIMPLE_HPP

#include <graph_analysis/gui/EdgeItem.hpp>
#include <QPainterPath>
#include <QColor>
#include <QGraphicsLineItem>

namespace graph_analysis {
namespace gui {
namespace graphitem {
namespace edges {

/**
 * \file Simple.hpp
 * \class Simple
 * \brief graphical edge implementation
 * \details specific to the diagram editor widget: straight direct edge
 *      holding a text label on top of its middle region
 */
class Simple : public graph_analysis::gui::EdgeItem
{
private:
    /**
     * \brief constructor; private since instances of this class only spawn via the EdgeTypeManager factory class
     * \param graphWidget managing widget
     * \param sourceNode source port vertex
     * \param sourceNodePortID source ID
     * \param targetNode target port vertex
     * \param targetNodePortID target ID
     * \param edge internal conceptual edge
     */
    Simple(GraphWidget* graphWidget, QGraphicsItem* source, QGraphicsItem* target, graph_analysis::Edge::Ptr edge);
public:
    /// empty constructor
    Simple() {}
    /// setter method for the graphical text label
    virtual void setLabel(const std::string& label);
    /// getter method for the graphical text label member field
    virtual graph_analysis::gui::items::EdgeLabel* getLabel() { return mpLabel; }
    /// destructor
    virtual ~Simple() {}
    /// adjusts graphical text label position
    void adjustLabel();
    /// adjusts edge segment length and starting and ending points
    virtual void adjust();

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
    /// qt mouse double click callback (toggles the scene focusing on this item)
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

    /// establishes where the edge line segment shall end at one of the extremities (computes intersection with the rectangular boundary of one of the endpoints)
    QPointF getIntersectionPoint(QGraphicsItem* item, const QLineF& line);

    // virtual methods
    virtual EdgeItem* createNewItem(GraphWidget* graphWidget,
            QGraphicsItem* source,
            QGraphicsItem* target,
            graph_analysis::Edge::Ptr edge) const
    { return new Simple(graphWidget, source, target, edge); }

    /// the graphical text label
    graph_analysis::gui::items::EdgeLabel* mpLabel;
    /// current qt drawing pen
    QPen mPen;
    /// the default qt drawing pen
    QPen mPenDefault;
    /// ending arrow tip
    QPolygonF mArrowHead;
    /// edge graphical line segment
    QLineF mLine;
    /// boolean flag: true when current edge is being the focused edge of the scene (has been double clicked); false otherwise
    bool mFocused;
    /// boolean flag: true when current edge is being the seleted edge of the scene (is being hovered on); false otherwise
    bool mSelected;
};

} // end namespace edges
} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_EDGES_SIMPLE_HPP
