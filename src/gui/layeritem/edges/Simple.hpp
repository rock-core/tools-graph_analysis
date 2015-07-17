#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_EDGES_SIMPLE_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_EDGES_SIMPLE_HPP

#include <graph_analysis/gui/EdgeItem.hpp>
#include <QPainterPath>
#include <QColor>
#include <QGraphicsLineItem>

namespace graph_analysis {
namespace gui {
namespace layeritem {
namespace edges {

class EdgeLabel;

class Simple : public graph_analysis::gui::EdgeItem
{
    Simple(GraphWidget* graphWidget, NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr vertex);
public:
    Simple() {}
    virtual EdgeLabel* getLabel() { return mpLabel; }
    virtual ~Simple() {}

    virtual void adjust();

protected:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const { return ::graph_analysis::gui::EdgeItem::shape(); }
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    QPointF getIntersectionPoint(NodeItem* item, const QLineF& line);


    virtual EdgeItem* createNewItem(GraphWidget* graphWidget, NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge) const { return new Simple(graphWidget, sourceNode, targetNode, edge); }

    EdgeLabel* mpLabel;
    QPen mPen;
    QPen mPenDefault;
    QGraphicsLineItem* mpLine;
    QPolygonF mArrowHead;
    QLineF mLine;

};

} // end namespace edges
} // end namespace layeritem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_EDGES_SIMPLE_HPP
