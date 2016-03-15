#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_CLUSTER_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_CLUSTER_HPP

#include <vector>
#include <QPen>
#include <QGraphicsWidget>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/items/Label.hpp>
// remove this include when "ConnectionRequest" is gone
#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {
namespace graphitem {

class ConnectionRequest
{
public:
    ConnectionRequest()
        : mpFrom()
        , mOpen(false)
    {}

    ConnectionRequest(const Vertex::Ptr& fromVertex)
        : mpFrom(fromVertex)
        , mOpen(true)
    {
        LOG_DEBUG_S << "CREATE Connection request from: " << fromVertex->toString();
    }

    bool isOpen() const { return mOpen; }
    void close() { LOG_DEBUG_S << "CLOSE REQUEST"; mOpen = false; }

    Vertex::Ptr getFrom() const { return mpFrom; }

private:
    Vertex::Ptr mpFrom;
    bool mOpen;
};

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
    virtual ~Cluster();

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

    /// calls a qt routine for warning the scene of upcoming graphical changes; the method it internally calls is otherwise protected and inaccessible to unrelated classes
    inline void prepareChange() { prepareGeometryChange(); }

protected:

    /// qt hovering LEAVE callback
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void dropEvent(QGraphicsSceneDragDropEvent* event);

private:
    /// main node text label
    items::Label *mpLabel;

    std::map<std::string, QGraphicsTextItem*> mLabels;

    // Store an open drag drop connection event for later handling
    ConnectionRequest mConnectionRequest;

    QGraphicsTextItem* getOrCreateLabel(const std::string& label, QGraphicsItem* parent = 0);
};

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_CLUSTER_HPP
