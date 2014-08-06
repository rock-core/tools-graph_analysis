#ifndef OMVIZ_GRAPHWIDGET_GRAPHITEM_SIMPLE_HPP
#define OMVIZ_GRAPHWIDGET_GRAPHITEM_SIMPLE_HPP

#include <omviz/graphwidget/NodeItem.hpp>

namespace omviz {
namespace graphitem {

class Simple : public omviz::NodeItem
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
} // end namespace omviz
#endif // OMVIZ_GRAPHWIDGET_GRAPHITEM_SIMPLE_HPP
