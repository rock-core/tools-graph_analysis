#ifndef GRAPH_ANALYSIS_GUI_ITEMS_LABEL_HPP
#define GRAPH_ANALYSIS_GUI_ITEMS_LABEL_HPP

#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/PortVertex.hpp>
#include <iostream>

namespace graph_analysis {
namespace gui {

class NodeItem;
class GraphWidget;

namespace items {

/**
 * \file Label.hpp
 * \class Label
 * \brief graphical node text label and port text label implementation
 * \details specific to the diagram editor widget: editable text representation (disregards the ENTER key)
 */
class Label : public QGraphicsTextItem
{
private:
    // managing graph widget
    GraphWidget *mpGraphWidget;
    // associated portID; negative if it pertains to a cluster node; positive if it is the label of a port vertex
    int mPortID;
    // internal for deciding whether the mouse move event path is long enough to initiate a drag move
    QPointF mDragStartPosition;
public:
    /**
     * \brief constructor
     * \param label text to be displayed
     * \param item parent NodeItem implementing instance
     * \param graphWidget managing widget
     * \param portID assigned ID; defaults to PortVertex::INVALID_PORT_ID (denoting label of a cluster node)
     */
    Label(const std::string& label, QGraphicsItem* item, GraphWidget *graphWidget = 0, int portID = PortVertex::INVALID_PORT_ID);
    /**
     * \brief toggles direct editing options of the displayed text
     * \param on boolean flag - true when the editing mode is to be turned on; false otherwise
     * \param selectAll boolean flag - true when in editing mode the whole text shall be initially automatically selected; false otherwise
     */
    void setTextInteraction(bool on, bool selectAll = false);
    /// getter method: retrieves the assigned ID
    int getPortID() { return mPortID; }

protected:
    /// qt mouse double-click callback
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);
    /// qt key pressing callback
    void keyPressEvent(::QKeyEvent* event);
    /// qt focus LEAVE callback
    void focusOutEvent(QFocusEvent* event);
    /// qt mouse move callback
    void mouseMoveEvent(::QGraphicsSceneMouseEvent *event);
    /// qt mouse press callback
    void mousePressEvent(::QGraphicsSceneMouseEvent *event);
    /// qt mouse drag start callback
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    /// qt mouse drag move callback
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    /// qt dropping callback
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    /// qt item changed callback
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
};

} // end namespace items
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_ITEMS_LABEL_HPP
