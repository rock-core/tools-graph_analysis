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
    void setPortLabel(NodeItem::portID_t portID, const std::string& label);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { return new Resource(graphWidget, vertex); }

    unsigned  getPortCount() { return mLabels.size(); }
    graph_analysis::Vertex::Ptr getPort(NodeItem::portID_t portID);
    portID_t addPort(Vertex::Ptr node);
    void removePort(NodeItem::portID_t portID);
    void syncLabel(NodeItem::portID_t portID);
    void prepareChange() { prepareGeometryChange(); }
    QPolygonF   portBoundingPolygon (NodeItem::portID_t portID);
    QRectF      portBoundingRect    (NodeItem::portID_t portID);

    void releaseFocus();
    void updateWidth (bool active = true);
    void updateHeight();
    void swapPorts(NodeItem::portID_t port1, NodeItem::portID_t port2);
    void removePorts();
    void unselect();
    void shiftPortUp(NodeItem::portID_t portID);
    void shiftPortDown(NodeItem::portID_t portID);

    Labels      getLabels()     { return mLabels;   }
    Vertices    getVertices()   { return mVertices; }

protected:
    void grabFocus();
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);
    void mousePressEvent(::QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(::QGraphicsSceneMouseEvent* event);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void dieOnPort(NodeItem::portID_t  portID, const std::string& caller = std::string());
    void displaceOutputPorts(qreal delta);
    void shiftOutputPorts(qreal delta);
    void recomputeMaxInputPortWidth(void);
    void recomputeMaxOutputPortWidth(void);

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
    bool mHeightAdjusted;
    NodeItem::portID_t mID;
    NodeItem::portID_t mInPorts, mOutPorts;
    qreal mMaxInputPortWidth, mMaxOutputPortWidth, mSeparator;
};

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP
