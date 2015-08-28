#ifndef GRAPH_ANALYSIS_GUI_NODEITEM_HPP
#define GRAPH_ANALYSIS_GUI_NODEITEM_HPP

#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QList>
#include <set>
#include <vector>
#include <string>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/gui/graphitem/Label.hpp>

#define LABEL_SWAPPING

namespace graph_analysis {
namespace gui {

namespace graphitem {
    class Label;
} // end namespace graphitem

class EdgeItem;
class GraphWidget;
class QGraphicsSceneMouseEvent;

/**
 * \file NodeItem.hpp
 * \class NodeItem
 * \brief graphical node representation interface
 * \details used as polymorphic base for several graphical node implementations
 */
class NodeItem : public QGraphicsItemGroup
{
public:
    typedef long long id_t; // counter datatype for attributing ID-s to features in the case of implementing cluster node items
    typedef std::map<id_t, graphitem::Label*> Labels; // map of features labels in a cluster node
    typedef std::pair<id_t, graphitem::Label*> Tuple; // item of a map of features labels in a cluster node
    typedef std::map<id_t, graph_analysis::Vertex::Ptr> Vertices; // map of feature vertices in a cluster node
    typedef std::pair<id_t, graph_analysis::Vertex::Ptr> VTuple; // item of map of feature vertices in a cluster node
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

    /**
     * Get the center position of this node item
     * The default implementation uses the boundingRect() and pos() to compute this information
     * \return center position of this node item in the scene
     */
    virtual QPointF getCenterPosition() const;

    /// fancy toString method: prepends the pointer address to the underlying vertex toString()
    std::string getId() const;

    /// getter method for retrieving the underlying conceptual graph vertex
    graph_analysis::Vertex::Ptr getVertex() { return mpVertex; }
    /// setter method for updating the underlying conceptual graph vertex
    void setVertex(graph_analysis::Vertex::Ptr vertex) { mpVertex = vertex; }
    /// getter method for retrieving the parent managing graph widget
    GraphWidget* getGraphWidget() { return mpGraphWidget; }

    /// virtual methods
    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { throw std::runtime_error("graph_analysis::gui::NodeItem::createNewItem is not reimplemented"); }

    virtual QPolygonF featureBoundingPolygon   (id_t)  { throw std::runtime_error("graph_analysis::gui::NodeItem::featureBoundingPolygon is not reimplemented");    }
    virtual QRectF    featureBoundingRect      (id_t)  { throw std::runtime_error("graph_analysis::gui::NodeItem::featureBoundingRect is not reimplemented");       }
    virtual void setFeatureLabel(id_t, const std::string&) { throw std::runtime_error("graph_analysis::gui::NodeItem::setFeatureLabel is not reimplemented"); }
    virtual void unselect() { throw std::runtime_error("graph_analysis::gui::NodeItem::unselect is not reimplemented"); }

    virtual void changeLabel(const std::string&)  { throw std::runtime_error("graph_analysis::gui::NodeItem::changeLabel is not reimplemented");  }
    virtual void updateLabel()                          { throw std::runtime_error("graph_analysis::gui::NodeItem::updateLabel is not reimplemented");  }
    virtual void setFeatureCount(unsigned)                 { throw std::runtime_error("graph_analysis::gui::NodeItem::setFeatureCount is not reimplemented"); }
    virtual unsigned  getFeatureCount()                    { throw std::runtime_error("graph_analysis::gui::NodeItem::getFeatureCount is not reimplemented"); }
    virtual id_t  addFeature(Vertex::Ptr)              { throw std::runtime_error("graph_analysis::gui::NodeItem::addFeature is not reimplemented");      }
    virtual void removeFeature(id_t)            { throw std::runtime_error("graph_analysis::gui::NodeItem::removeFeature is not reimplemented");   }
    virtual graph_analysis::Vertex::Ptr getFeature(id_t)    {throw std::runtime_error("graph_analysis::gui::NodeItem::getFeature is not reimplemented");}
    virtual std::string  getLabel()                     { throw std::runtime_error("graph_analysis::gui::NodeItem::getLabel is not reimplemented");     }
    virtual void syncLabel(id_t)             { throw std::runtime_error("graph_analysis::gui::NodeItem::syncLabel is not reimplemented");    }
    virtual void shiftFeatureUp(id_t)           { throw std::runtime_error("graph_analysis::gui::NodeItem::shiftFeatureUp is not reimplemented");  }
    virtual void shiftFeatureDown(id_t)         { throw std::runtime_error("graph_analysis::gui::NodeItem::shiftFeatureDown is not reimplemented");}
    virtual void prepareChange()                        { throw std::runtime_error("graph_analysis::gui::NodeItem::prepareChange is not reimplemented");}
    virtual void releaseFocus ()                        { throw std::runtime_error("graph_analysis::gui::NodeItem::releaseFocus is not reimplemented"); }
    virtual void updateWidth(bool active = true)                        { throw std::runtime_error("graph_analysis::gui::NodeItem::updateWidth  is not reimplemented"); }
    virtual void updateHeight()                       { throw std::runtime_error("graph_analysis::gui::NodeItem::updateHeight is not reimplemented"); }
    virtual void swapFeatures(id_t, id_t)          { throw std::runtime_error("graph_analysis::gui::NodeItem::swapFeatures is not reimplemented");    }
    virtual void removeFeatures()                          { throw std::runtime_error("graph_analysis::gui::NodeItem::removeFeatures is not reimplemented");  }
    virtual Labels      getLabels()     { throw std::runtime_error("graph_analysis::gui::NodeItem::getLabels is not reimplemented");   }
    virtual Vertices    getVertices()   { throw std::runtime_error("graph_analysis::gui::NodeItem::getVertices is not reimplemented"); }

protected:
    /// qt item change callback
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);

    /// underlying graph vertex
    graph_analysis::Vertex::Ptr mpVertex;
    /// target position (updated by the force field algorithm)
    QPointF mNewPos;
    /// parent managing graph widget
    GraphWidget* mpGraphWidget;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_NODEITEM_HPP
