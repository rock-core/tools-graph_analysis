#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_LABEL_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_LABEL_HPP

#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>



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
    QPointF mDragStartPosition;
public:
    Label(const std::string& label, QGraphicsItem* item, GraphWidget *graphWidget = 0, int portID = -1);
    void setTextInteraction(bool on, bool selectAll = false);
public slots:
    void showContextMenu(const QPoint& pos);
    void changePortLabel();

protected:
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);
    void keyPressEvent(::QKeyEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void mouseMoveEvent(::QGraphicsSceneMouseEvent *event);
    void mousePressEvent(::QGraphicsSceneMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
//    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
};

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_LABEL_HPP
