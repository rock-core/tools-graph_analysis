#ifndef GRAPH_ANALYSIS_GUI_EDGEITEMSIMPLE_HPP
#define GRAPH_ANALYSIS_GUI_EDGEITEMSIMPLE_HPP

#include <graph_analysis/gui/EdgeItemBase.hpp>

namespace graph_analysis {
namespace gui {

/**
 *
 * simplest possible implementation of EdgeItemBase: just a line from source to target
 *
 */
class EdgeItemSimple : public EdgeItemBase
{
    friend class EdgeItemTypeManager;

protected:
    EdgeItemSimple() {}

public:
    EdgeItemSimple(GraphWidget* graphWidget, graph_analysis::Edge::Ptr edge,
                   QGraphicsItem* parent);
    ~EdgeItemSimple();
    virtual int type() const;

    void adjustEdgePositioning();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;

    int mArrowSize;
    QGraphicsTextItem* mpLabel;
    QGraphicsTextItem* mpClassName;
    QGraphicsPolygonItem* mpMultiLine;
    QGraphicsPolygonItem* mpArrowHead;

    EdgeItemBase* createNewItem(GraphWidget* graphWidget,
                const graph_analysis::Edge::Ptr& edge,
                QGraphicsItem* parent) const;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
