#ifndef GRAPH_ANALYSIS_GUI_EDGEITEM_HPP
#define GRAPH_ANALYSIS_GUI_EDGEITEM_HPP

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QPainter>
#include <math.h>
#include <graph_analysis/Edge.hpp>

namespace graph_analysis {
namespace gui {

class GraphWidget;

namespace items {
    class EdgeLabel;
}
/**
 * \file EdgeItem.hpp
 * \class EdgeItem
 * \brief graphical edge representation interface
 * \details used as polymorphic base for several graphical edge implementations
 */
class EdgeItem : public QGraphicsItemGroup
{
public:
    /// empty constructor
    EdgeItem() {}
    /// destructor
    ~EdgeItem() {}

    /**
     * \brief constructor
     * \param graphWidget the parent and managing graph widget
     * \param sourceNode the edge source endpoint vertex
     * \param destNode the edge target endpoint vertex
     * \param edge the internal conceptual edge
     */
    EdgeItem(GraphWidget* graphWidget, QGraphicsItem* source, QGraphicsItem* target, graph_analysis::Edge::Ptr edge);

    /// getter method to retrieve source node
    QGraphicsItem* sourceItem() const { return mpSourceItem; }
    /// getter method to retrieve target node
    QGraphicsItem* targetItem() const { return mpTargetItem; }

    /// updates edge graphics: edge lenght and endpoints positioning
    virtual void adjust();

    enum { Type = UserType + 2 };
    /// getter method to retrieve constant
    int type() const { return Type; }
    /// getter method to retrieve  the internal conceptual edge
    graph_analysis::Edge::Ptr getEdge() { return mpEdge; }

    // virtual methods
    virtual EdgeItem* createNewItem(GraphWidget* graphWidget,
            QGraphicsItem* source,
            QGraphicsItem* target,
            graph_analysis::Edge::Ptr edge) const
    { throw std::runtime_error("graph_analysis::gui::EdgeItem::createNewItem (@4 args) is not reimplemented"); }

    virtual void setLabel(const std::string&)  { throw std::runtime_error("graph_analysis::gui::EdgeItem::setLabel is not reimplemented");  }
    virtual graph_analysis::gui::items::EdgeLabel* getLabel() {  throw std::runtime_error("graph_analysis::gui::EdgeItem: edge label is not implemented at this level"); }
    virtual void releaseFocus()   { throw std::runtime_error("graph_analysis::gui::EdgeItem::releaseFocus is not reimplemented");}
    virtual void adjustLabel()    { throw std::runtime_error("graph_analysis::gui::EdgeItem::adjustLabel is not reimplemented"); }

protected:
    /// parent and managing graph widget
    GraphWidget* mpGraphWidget;
    /// source edge-endpoint
    QGraphicsItem* mpSourceItem;
    /// target edge-endpoint
    QGraphicsItem* mpTargetItem;

    /// internal conceptual edge on whose basis the edge's been constructed
    graph_analysis::Edge::Ptr mpEdge;

    /// in-scene edge source point
    QPointF mSourcePoint;
    /// in-scene edge target point
    QPointF mTargetPoint;
    /// arrow tip size
    qreal mArrowSize;

    /// retrieves the bounding rectangular box around the area occupied by the node in the scene
    virtual QRectF boundingRect() const;
    /// qt node painting method - here the node components get placed in the scene
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    /// retrieves the path of the bounding rectangular box around the area occupied by the node in the scene

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_EDGEITEM_HPP
