#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_SIMPLE_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_SIMPLE_HPP

#include <graph_analysis/gui/NodeItem.hpp>

namespace graph_analysis {
namespace gui {
namespace graphitem {

class Simple : public graph_analysis::gui::NodeItem
{
    Simple(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);
public:
    Simple() {}
    virtual ~Simple() {}

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { return new Simple(graphWidget, vertex); }
};

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_SIMPLE_HPP
