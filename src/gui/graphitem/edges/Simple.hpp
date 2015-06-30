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

class EdgeLabel;

class Simple : public graph_analysis::gui::EdgeItem
{
    Simple(GraphWidget* graphWidget, NodeItem* sourceNode, int sourceNodePortID, NodeItem* targetNode, int targetNodePortID, graph_analysis::Edge::Ptr vertex);
public:
    Simple() {}
    EdgeLabel* getLabel() { return mpLabel; }
    virtual ~Simple() {}
    int getSourcePortID() { return mSourceNodePortID; }
    int getTargetPortID() { return mTargetNodePortID; }
    virtual void adjust();

protected:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const { return ::graph_analysis::gui::EdgeItem::shape(); }
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    QPointF getIntersectionPoint(NodeItem* item, const QLineF& line, int portID = -1);


    virtual EdgeItem* createNewItem(GraphWidget* graphWidget, NodeItem* sourceNode, int sourceNodePortID, NodeItem* targetNode, int targetNodePortID, graph_analysis::Edge::Ptr edge) const { return new Simple(graphWidget, sourceNode, sourceNodePortID, targetNode, targetNodePortID, edge); }

    EdgeLabel* mpLabel;
    QPen mPen;
    QPen mPenDefault;
    QGraphicsLineItem* mpLine;
    QPolygonF mArrowHead;
    QLineF mLine;
    int mSourceNodePortID;
    int mTargetNodePortID;

};

} // end namespace edges
} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_EDGES_SIMPLE_HPP
