#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_CLUSTER_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_CLUSTER_HPP

#include <vector>
#include <QPen>
#include <QGraphicsWidget>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/items/Label.hpp>

namespace graph_analysis {
namespace gui {
namespace graphitem {

/**
 * \file Cluster.hpp
 * \class Cluster
 * \brief graphical cluster node representation
 * \details implements the NodeItem interface into a movable cluster node with dynamic, editable
 *      input and output ports, with operations and properties
 */
class Cluster : public graph_analysis::gui::NodeItem
{
private:
    /// constructor; private since instances of this class only spawn via the NodeTypeManager factory class
    Cluster(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);
public:
    /// empty constructor
    Cluster() {}
    /// destructor
    virtual ~Cluster() {};

    /**
     * \brief changes node main label to the provided label; syncs also the mpVertex internal label
     * \param label the provided new label
     */
    void changeLabel(const std::string& label);
    /**
     * \brief getter method for retrieving the currently displayed main text label
     * \return the current string label
     */
    std::string getLabel() { return mpLabel->toPlainText().toStdString(); }
    ///syncs the graphical text label from the internal mpVertex label
    void updateLabel();
    /**
     * \brief sets the label of a given feature
     * \param id ID of the concerned feature component
     * \param label new text label to be stored and displayed in place
     */
    void setFeatureLabel(NodeItem::id_t id, const std::string& label);

    /// retrieves the bounding rectangular box around the area occupied by the node in the scene
    virtual QRectF boundingRect() const;
    /// retrieves the path of the bounding rectangular box around the area occupied by the node in the scene
    virtual QPainterPath shape() const;
    /// qt node painting method - here the node components get placed in the scene
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    /// node cloning/spawning method used by the factory to produce new nodes
    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { return new Cluster(graphWidget, vertex); }

    /// returns the no. of features currently in the node
    unsigned  getFeatureCount() { return mLabels.size(); }
    /**
     * \brief getter for the feature of a given ID
     * \param id the requested ID to look up
     * \return the feature vertex having the requested feature ID
     */
    graph_analysis::Vertex::Ptr getFeature(NodeItem::id_t id);
    /// adds a new given feature vertex to the node features (and displays it)
    id_t addFeature(Vertex::Ptr vertex);
    /// removes a feature vertex (indicated by feature ID) from the node features (removes it from the scene too)
    void removeFeature(NodeItem::id_t id);
    /// syncs the updated text label of the feature label in the scene with the corresponding internal feature vertex label (both being indicated by the same provided feature ID)
    void syncLabel(NodeItem::id_t id);
    /// calls a qt routine for warning the scene of upcoming graphical changes; the method it internally calls is otherwise protected and inaccessible to unrelated classes
    inline void prepareChange() { prepareGeometryChange(); }
    /// retrieves the bounding polygon around the area occupied by the feature node specified by the given ID in the scene
    QPolygonF   featureBoundingPolygon (NodeItem::id_t id);
    /// retrieves the bounding rectangular box around the area occupied by the feature node specified by the given ID in the scene (it does that using featureBoundingRect)
    QRectF      featureBoundingRect    (NodeItem::id_t id);

    /// willingly gives up scene focus
    void releaseFocus();
    /**
     * \brief updates node width related essential internal variables
     * \param active boolean flag for updating GUI too; when set changes also affect the node graphical representation (i.e. the node horizontally shrinks or expands in the scene)
     */
    void updateWidth (bool active = true);
    /// recomputes node's height; makes the node vertically shrink or expand
    void updateHeight(void);
    /// swaps 2 features positions indicated by their IDs
    void swapFeatures(NodeItem::id_t feature1, NodeItem::id_t feature2);
    /// removes all features
    void removeFeatures();
    /// willingly gives up hovering selection
    void unselect();
    /// pushes indicated feature (by its feature ID) one position upper (unless it's the top most feature)
    void shiftFeatureUp(NodeItem::id_t id);
    /// pushes indicated feature (by its feature ID) one position lower (unless it's the bottom most feature)
    void shiftFeatureDown(NodeItem::id_t id);

    /// getter method: retrieves the grahical (feature) labels map
    Labels      getLabels()     { return mLabels;   }
    /// getter method: retrieves the conceptual feature vertices labels map
    Vertices    getVertices()   { return mVertices; }

    /// updates the temporary text currently being displayed on the Status Bar in the main viewWidget
    void updateStatus(const std::string& message = std::string(), int timeout = 0);

protected:
    /// shifts down one lane all operations features - useful e.g. when a new property is inserted on top of them at runtime
    void pushDownOperations(NodeItem::id_t times = 1);
    /// shifts down one lane all non-ports features (properties and operations) - useful e.g. when a new port is inserted on top of them at runtime
    void pushDownNonPortFeatures(NodeItem::id_t times = 1);

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
     * \brief checks for the feature existence (indicated by its feature ID); throws on reason: the given 'id' is not registered in the feature maps
     * \param id ID of the needed feature
     * \param caller text name of the caller method (for which the error checking is being performed)
     */
    void dieOnFeature(NodeItem::id_t  id, const std::string& caller = std::string());
    /// shifts right all output ports with delta; counting from the left cluster node boundary
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

    //virtual void dropEvent(QGraphicsSceneDragDropEvent* event);
private:
    /// background child widget
    QGraphicsWidget *mpBoard;
    /// main node text label
    items::Label *mpLabel;
    /// ports text label; displays "Ports:" in italics
    items::Label *mpPortsLabel;
    /// properties text label; displays "Properties:" in italics
    items::Label *mpPropsLabel;
    /// operations text label; displays "Operations:" in italics
    items::Label *mpOpsLabel;
    /// constant width in the scene of text label "Ports:"
    qreal mWidthPortsLabel;
    /// constant width in the scene of text label "Properties:"
    qreal mWidthPropsLabel;
    /// constant width in the scene of text label "Operations:"
    qreal mWidthOpsLabel;
    /// feature labels map
    Labels mLabels;
    /// conceptual feature vertices
    Vertices mVertices;
    /// qt drawing pen
    QPen mPen;
    /// default qt drawing pen
    QPen mPenDefault;
    /// boolean flag: true when current node is being the focused node of the scene (has been double clicked); false otherwise
    bool mFocused;
    /// boolean flag: true when current node is being the seleted node of the scene (is being hovered on); false otherwise
    bool mSelected;
    /// boolean flag: true when extra bottom padding has been already added (preventing the bottom most features from getting graphically represented outside of their node); false otherwise
    bool mHeightAdjusted;
    /// current auto-incremented feature ID
    NodeItem::id_t mID;
    /// input ports counter
    NodeItem::id_t mInPorts;
    /// output ports counter
    NodeItem::id_t mOutPorts;
    /// properties counter
    NodeItem::id_t mProps;
    /// operations counter
    NodeItem::id_t mOps;
    /// current max width of input ports
    qreal mMaxInputPortWidth;
    /// current max width of output ports
    qreal mMaxOutputPortWidth;
    /// current width of ports separator (displacement between the 2 types of ports)
    qreal mSeparator;
};

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_CLUSTER_HPP
