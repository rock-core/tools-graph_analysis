#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_LABEL_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_LABEL_HPP

#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

#include <graph_analysis/Vertex.hpp>
#include <iostream>

namespace graph_analysis {
namespace gui {

class NodeItem;
class GraphWidget;

namespace graphitem {

class Label : public QGraphicsTextItem
{
private:
    GraphWidget *mpGraphWidget;
    int mPortID;
    graph_analysis::Vertex::Ptr mNode;
    QPointF mDragStartPosition;
public:
    Label(const std::string& label, QGraphicsItem* item, GraphWidget *graphWidget = 0, int portID = -1, graph_analysis::Vertex::Ptr node = graph_analysis::Vertex::Ptr(new graph_analysis::Vertex()));
    void setTextInteraction(bool on, bool selectAll = false);
    int getPortID() { return mPortID; }
    graph_analysis::Vertex::Ptr getNode() { return mNode; }

protected:
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);
    void keyPressEvent(::QKeyEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void mouseMoveEvent(::QGraphicsSceneMouseEvent *event);
    void mousePressEvent(::QGraphicsSceneMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
};

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_LABEL_HPP
