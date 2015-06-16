#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP

#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <QPen>

namespace graph_analysis {
namespace gui {
namespace graphitem {

class Label;

class Resource : public graph_analysis::gui::NodeItem
{
    Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);
    void changeLabel(const std::string& label);
    void updateLabel();
public:
    Resource() {}
    virtual ~Resource() {};

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { return new Resource(graphWidget, vertex); }

    void setPortCount(int portCount)    { mPortCount = portCount; }
    int  getPortCount()                 { return mPortCount; }

    void addPort(NodeItem* node);

protected:
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);
    void mousePressEvent(::QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(::QGraphicsSceneMouseEvent* event);

    //virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

//    void keyPressEvent(QKeyEvent* event);


private:
    Label* mLabel;
    QPen mPen;
    QPen mPenDefault;
    int mPortCount;
};

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP
