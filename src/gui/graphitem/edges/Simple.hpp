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
    Simple(NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr vertex);
public:
    Simple() {}
    virtual ~Simple() {}

    virtual void adjust(); 

protected:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const { return ::graph_analysis::gui::EdgeItem::shape(); }
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    QPointF getIntersectionPoint(NodeItem* item, const QLineF& line);


    virtual EdgeItem* createNewItem(NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge) const { return new Simple(sourceNode, targetNode, edge); }

    EdgeLabel* mpLabel;
    QPen mPen;
    QGraphicsLineItem* mpLine;
    QPolygonF mArrowHead;
    QLineF mLine;

};

} // end namespace edges
} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_EDGES_SIMPLE_HPP
