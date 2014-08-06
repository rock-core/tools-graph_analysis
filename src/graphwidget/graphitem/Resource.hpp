#ifndef OMVIZ_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP
#define OMVIZ_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP

#include <omviz/graphwidget/NodeItem.hpp>

namespace omviz {
namespace graphitem {

class Label;

class Resource : public omviz::NodeItem
{
    Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);
public:
    Resource() {}
    virtual ~Resource() {};

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { return new Resource(graphWidget, vertex); }

protected:
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);
    void mousePressEvent(::QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(::QGraphicsSceneMouseEvent* event);

//    void keyPressEvent(QKeyEvent* event);


private:
    Label* mLabel;
};

} // end namespace graphitem
} // end namespace omviz
#endif // OMVIZ_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP
