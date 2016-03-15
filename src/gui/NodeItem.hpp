#ifndef GRAPH_ANALYSIS_GUI_NODEITEM_HPP
#define GRAPH_ANALYSIS_GUI_NODEITEM_HPP

#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QList>
#include <QDropEvent>
#include <set>
#include <vector>
#include <string>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/gui/VertexGetter.hpp>

#define LABEL_SWAPPING

class QGraphicsSceneMouseEvent;

namespace graph_analysis {
namespace gui {

namespace items {
    class Label;
    class Feature;
} // end namespace items

class EdgeItem;
class GraphWidget;

/**
 * \file NodeItem.hpp
 * \class NodeItem
 * \brief graphical node representation interface
 * \details used as polymorphic base for several graphical node implementations
 */
class NodeItem : public QGraphicsItemGroup, public VertexGetter
{
public:
    typedef size_t id_t; // counter datatype for attributing ID-s to features in the case of implementing cluster node items
    typedef std::map<id_t, items::Label*> Labels; // map of features labels in a cluster node
    typedef std::map<id_t, items::Feature*> Features; // map of features 
    typedef std::pair<id_t, items::Label*> Tuple; // item of a map of features labels in a cluster node
    typedef std::map<id_t, Vertex::Ptr> Vertices; // map of feature vertices in a cluster node
    typedef std::pair<id_t, Vertex::Ptr> VTuple; // item of map of feature vertices in a cluster node
protected:
    /**
     * \brief constructor
     * \param graphWidget the parent and managing graph widget
     * \param vertex the internal conceptual vertex
     */
    NodeItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);

public:
    /// empty constructor
    NodeItem() {}

    /// destructor
    virtual ~NodeItem() {};

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    /// computes the force field values for each edge (w.r.t. the force field automatic re-arranging algorithm)
    void calculateForces();

    /// actuator method the same force field (automatic re-arranging) algorithm
    void advance(int phase) { if(phase == 2) advance(); }

    /**
     * Advance the NodeItem
     * \return True if advancing resulted in an actual change, false otherwise
     */
    virtual bool advance();

    /// fancy toString method: prepends the pointer address to the underlying vertex toString()
    std::string getId() const;

    /// getter method for retrieving the underlying conceptual graph vertex
    graph_analysis::Vertex::Ptr getVertex() const { return mpVertex; }
    /// setter method for updating the underlying conceptual graph vertex
    void setVertex(graph_analysis::Vertex::Ptr vertex) { mpVertex = vertex; }
    /// getter method for retrieving the parent managing graph widget
    GraphWidget* getGraphWidget() const { return mpGraphWidget; }

    /// virtual methods
    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { throw std::runtime_error("graph_analysis::gui::NodeItem::createNewItem is not reimplemented"); }

    virtual void setFeatureLabel(id_t, const std::string&) { throw std::runtime_error("graph_analysis::gui::NodeItem::setFeatureLabel is not reimplemented"); }
    virtual void unselect() { throw std::runtime_error("graph_analysis::gui::NodeItem::unselect is not reimplemented"); }

    virtual void setLabel(const std::string& label)  { mpVertex->setLabel(label); }
    virtual std::string getLabel() { return mpVertex->getLabel(); }

    virtual size_t getFeatureCount() { return mFeatures.size(); }
    virtual id_t addFeature(items::Feature* feature);
    virtual void removeFeature(id_t);
    virtual void removeFeatures();
    virtual items::Feature* getFeature(id_t) const;

    virtual void prepareChange()                        { throw std::runtime_error("graph_analysis::gui::NodeItem::prepareChange is not reimplemented");}
    virtual void releaseFocus ()                        { throw std::runtime_error("graph_analysis::gui::NodeItem::releaseFocus is not reimplemented"); }
    virtual void swapFeatures(id_t, id_t)          { throw std::runtime_error("graph_analysis::gui::NodeItem::swapFeatures is not reimplemented");    }

    virtual Labels      getLabels()     { throw std::runtime_error("graph_analysis::gui::NodeItem::getLabels is not reimplemented");   }
    virtual Vertices    getVertices()   { throw std::runtime_error("graph_analysis::gui::NodeItem::getVertices is not reimplemented"); }

protected:
    /// underlying graph vertex
    graph_analysis::Vertex::Ptr mpVertex;

    /// Associated features
    std::vector<items::Feature*> mFeatures;

    /// target position (updated by the force field algorithm)
    QPointF mNewPos;
    /// parent managing graph widget
    GraphWidget* mpGraphWidget;
    /// qt item change callback
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);

    virtual void dropEvent(QGraphicsSceneDragDropEvent* event);

    /** Check if any feature of this item is under mouse pointer
     * When pointer is over multiple feature, it returns the first available.
     * To avoid this undesired behaviour make sure boundingRect of items do not
     * overlap
     * \return feature that is under the mouse
     */
    const items::Feature* getPointedAtFeature() const;

    /**
     * Since each feature is associated with a vertex it return the
     * corresponding vertex
     * \see getPointedAtFeature
     * \return underlying vertex pointed at
     */
    Vertex::Ptr getPointedAtVertex() const;

    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_NODEITEM_HPP
