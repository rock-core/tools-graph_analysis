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

    ///syncs the graphical text label from the internal mpVertex label
    void updateLabel();

    /// retrieves the bounding rectangular box around the area occupied by the node in the scene
    virtual QRectF boundingRect() const;
    /// retrieves the path of the bounding rectangular box around the area occupied by the node in the scene
    virtual QPainterPath shape() const;
    /// qt node painting method - here the node components get placed in the scene
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    /// node cloning/spawning method used by the factory to produce new nodes
    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { return new Cluster(graphWidget, vertex); }

    /// syncs the updated text label of the feature label in the scene with the corresponding internal feature vertex label (both being indicated by the same provided feature ID)
    //void syncLabel(NodeItem::id_t id);

    /// calls a qt routine for warning the scene of upcoming graphical changes; the method it internally calls is otherwise protected and inaccessible to unrelated classes
    inline void prepareChange() { prepareGeometryChange(); }
    /// retrieves the bounding polygon around the area occupied by the feature node specified by the given ID in the scene
    QPolygonF   featureBoundingPolygon (NodeItem::id_t id);
    /// retrieves the bounding rectangular box around the area occupied by the feature node specified by the given ID in the scene (it does that using featureBoundingRect)
    QRectF      featureBoundingRect    (NodeItem::id_t id);

    
    QRectF      featureBoundingRect    (items::Feature* feature);

    /// willingly gives up scene focus
    void releaseFocus();
    /**
     * \brief updates node width related essential internal variables
     * \param active boolean flag for updating GUI too; when set changes also affect the node graphical representation (i.e. the node horizontally shrinks or expands in the scene)
     */
    void updateWidth (bool active = true) {}

    /// recomputes node's height; makes the node vertically shrink or expand
    void updateHeight(void) {}

    /// getter method: retrieves the grahical (feature) labels map
    //Labels      getLabels()     { return mLabels;   }

    /// updates the temporary text currently being displayed on the Status Bar in the main viewWidget
    void updateStatus(const std::string& message = std::string(), int timeout = 0);

protected:
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

    /// qt hovering ENTER callback
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    /// qt hovering LEAVE callback
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent* event);
private:
    /// background child widget
    QGraphicsWidget *mpBoard;
    /// main node text label
    items::Label *mpLabel;

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
