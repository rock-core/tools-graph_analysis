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

/**
 * \class Resource
 * \file Resource.hpp
 * \brief graphical node representation class; implements the NodeItem interface
 */
class Resource : public graph_analysis::gui::NodeItem
{
private:
    /// constructor; private since instances of this class only spawn via the NodeTypeManager factory class
    Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);
public:
    /// empty constructor
    Resource() {}
    /// destructor
    virtual ~Resource() {};

    /**
     * \brief changes node main label to the provided label; syncs also the mpVertex internal label
     * \param label the provided new label
     */
    void changeLabel(const std::string& label);
    /**
     * \brief getter method for retrieving the currently displayed main text label
     * \return the current string label
     */
    std::string  getLabel() { return mLabel->toPlainText().toStdString(); }
    ///syncs the graphical text label from the internal mpVertex label
    void updateLabel();
    /**
     * \brief sets the label of a given port
     * \param portID ID of the concerned port component
     * \param label new text label to be stored and displayed in place
     */
    void setPortLabel(NodeItem::portID_t portID, const std::string& label);

    /// retrieves the bounding rectangular box around the area occupied by the node in the scene
    virtual QRectF boundingRect() const;
    /// retrieves the path of the bounding rectangular box around the area occupied by the node in the scene
    virtual QPainterPath shape() const;
    /// qt node painting method - here the node components get placed in the scene
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    /// node cloning/spawning method used by the factory to produce new nodes
    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { return new Resource(graphWidget, vertex); }

    /// returns the no. of ports currently in the node
    unsigned  getPortCount() { return mLabels.size(); }
    /**
     * \brief getter for the port of a given ID
     * \param portID the requested ID
     * \return the port vertex having the requested port ID
     */
    graph_analysis::Vertex::Ptr getPort(NodeItem::portID_t portID);
    /// adds a new given port vertex to the node ports (and displays it)
    portID_t addPort(Vertex::Ptr node);
    /// removes a port vertex (indicated by port ID) from the node ports (removes it from the scene too)
    void removePort(NodeItem::portID_t portID);
    /// syncs the updated text label of the port label in the scene with the corresponding internal port vertex label (both being indicated by the same provided port ID)
    void syncLabel(NodeItem::portID_t portID);
    /// calls a qt routine for warning the scene of upcoming graphical changes; the method it internally calls is otherwise protected and inaccessible to unrelated classes
    inline void prepareChange() { prepareGeometryChange(); }
    /// retrieves the bounding polygon around the area occupied by the port node specified by the given ID in the scene
    QPolygonF   portBoundingPolygon (NodeItem::portID_t portID);
    /// retrieves the bounding rectangular box around the area occupied by the port node specified by the given ID in the scene (it does that using portBoundingRect)
    QRectF      portBoundingRect    (NodeItem::portID_t portID);

    /// willingly gives up scene focus
    void releaseFocus();
    /**
     * \brief updates node width related essential internal variables
     * \param active boolean flag for updating GUI too; when set changes also affect the node graphical representation (i.e. the node horizontally shrinks or expands in the scene)
     */
    void updateWidth (bool active = true);
    /// recomputes node's height; makes the node vertically shrink or expand
    void updateHeight();
    /// swaps 2 ports positions indicated by their IDs
    void swapPorts(NodeItem::portID_t port1, NodeItem::portID_t port2);
    /// removes all ports
    void removePorts();
    /// willingly gives up hovering selection
    void unselect();
    /// pushes indicated port (by its port ID) one position upper (unless it's the top most port)
    void shiftPortUp(NodeItem::portID_t portID);
    /// pushes indicated port (by its port ID) one position lower (unless it's the bottom most port)
    void shiftPortDown(NodeItem::portID_t portID);

    /// getter method: retrieves the grahical (port) labels map
    Labels      getLabels()     { return mLabels;   }
    /// getter method: retrieves the conceptual port vertices labels map
    Vertices    getVertices()   { return mVertices; }

protected:
    /// used to claim scene focus (mainly when double-clicked)
    void grabFocus();
    /// qt mouse double-click callback
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);
    /// qt mouse press callback
    void mousePressEvent(::QGraphicsSceneMouseEvent* event);
    /// qt mouse release callback
    void mouseReleaseEvent(::QGraphicsSceneMouseEvent* event);
    /// qt focus ENTER callback
    void focusInEvent(QFocusEvent* event);
    /// qt focus LEAVE callback
    void focusOutEvent(QFocusEvent* event);
    /**
     * \brief checks for the port existence (indicated by its port ID); throws on reason: the given 'portID' is not registered in the port maps
     * \param portID ID of the needed port
     * \param caller text name of the caller method (for which the error checking is being performed)
     */
    void dieOnPort(NodeItem::portID_t  portID, const std::string& caller = std::string());
    /// shifts right all output ports with delta; counting from the left node boundary
    void displaceOutputPorts(qreal delta);
    /// shifts right all output ports with delta; takes effect relative to the current output ports displacement
    void shiftOutputPorts(qreal delta);
    /// recomputes the maximum input port width (by iterating through them one by one and inspecting the label width in the scene)
    void recomputeMaxInputPortWidth(void);
    /// recomputes the maximum output port width (by iterating through them one by one and inspecting the label width in the scene)
    void recomputeMaxOutputPortWidth(void);

    /// qt hovering ENTER callback
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    /// qt hovering LEAVE callback
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
    /// background child widget
    QGraphicsWidget *mpBoard;
    /// main node text label
    Label *mLabel;
    /// port labels map
    Labels mLabels;
    /// conceptual port vertices
    Vertices mVertices;
    /// qt drawing pen
    QPen mPen;
    /// default qt drawing pen
    QPen mPenDefault;
    /// boolean flag: true when current node is being the focused node of the scene (has been double clicked); false otherwise
    bool mFocused;
    /// boolean flag: true when current node is being the seleted node of the scene (is being hovered on); false otherwise
    bool mSelected;
    /// boolean flag: true when extra bottom padding has been already added (preventing the bottom most ports from getting graphically represented outside of their node); false otherwise
    bool mHeightAdjusted;
    /// current auto-incremented port ID
    NodeItem::portID_t mID;
    /// input ports counter
    NodeItem::portID_t mInPorts;
    /// output ports counter
    NodeItem::portID_t mOutPorts;
    /// current max width of input ports
    qreal mMaxInputPortWidth;
    /// current max width of output ports
    qreal mMaxOutputPortWidth;
    /// current width of ports separator (displacement between the 2 kinds of ports)
    qreal mSeparator;
};

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP
