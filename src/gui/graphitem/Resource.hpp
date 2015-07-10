#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP

#include <vector>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <QPen>
#include <QGraphicsWidget>

namespace graph_analysis {
namespace gui {
namespace graphitem {

class Label;

class Resource : public graph_analysis::gui::NodeItem
{
private:
    Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);
public:
    Resource() {}
    virtual ~Resource() {};

    void changeLabel(const std::string& label);
    std::string  getLabel() { return mLabel->toPlainText().toStdString(); }
    void updateLabel();
    void setPortLabel(int portID, const std::string& label);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { return new Resource(graphWidget, vertex); }

    int  getPortCount() { return mLabels.size(); }
    graph_analysis::Vertex::Ptr getPort(int portID);
    int addPort(Vertex::Ptr node);
    void removePort(int portID);
    void syncLabel(int portID);
    void prepareChange() { prepareGeometryChange(); }
    QPolygonF   portBoundingPolygon (int portID);
    QRectF      portBoundingRect    (int portID);

    void releaseFocus();
    void updateWidth ();
    void updateHeight();
    void swapPorts(int port1, int port2);
    void removePorts();
    void unselect();

    Labels      getLabels()     { return mLabels;   }
    Vertices    getVertices()   { return mVertices; }

protected:
    void grabFocus();
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);
    void mousePressEvent(::QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(::QGraphicsSceneMouseEvent* event);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void dieOnPort(int portID, const std::string& caller = "");

    //virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
//    void keyPressEvent(QKeyEvent* event);

private:
    QGraphicsWidget *mpBoard;
    Label *mLabel;
    Labels mLabels;
    Vertices mVertices;
    QPen mPen;
    QPen mPenDefault;
    bool mFocused;
    bool mSelected;
    unsigned long long mID;
};

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP
