#ifndef GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
#define GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H

#include "NodeItem.hpp"

#include <map>
#include <QIcon>
#include <QMainWindow>
#include <QGraphicsView>
#include <QStackedWidget>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/lemon/Graph.hpp>

namespace graph_analysis {
namespace io {
    class GVGraph;
}

namespace gui {

class GraphWidgetManager;

class NodeItem;
class EdgeItem;
class AddNodeDialog;
class ActionCommander;
class PropertyDialog;

/**
 *
 * \beginverbatim
    GraphWidget* widget = new GraphWidget;

    // Create vertices
    for(int i = 0; i < 1; ++i)
    {
        graph_analysis::Vertex::Ptr vertex(new graph_analysis::Vertex());
        widget->addVertex(vertex);
    }
 \endverbatim
 */

/**
 * \file GraphWidget.hpp
 * \class GraphWidget
 * \brief graph view widget interface
 * \details polymorphics base for the different kinds of graph widgets
 *      that make use of different kinds of node and edge items implementations
 */
class GraphWidget : public QGraphicsView
{
    friend class GraphWidgetManager;

public:
    typedef std::map<graph_analysis::Edge::Ptr, graph_analysis::Edge::Ptr> EdgeMap; // meant to map default edges to their correspondent initial edges in the base graph
    typedef std::map<graph_analysis::Edge::Ptr, EdgeItem*> EdgeItemMap; // maps conceptual edges to the graphical edges in the scene
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem*> NodeItemMap; // maps conceptual cluster vertices to graphical nodes in the scene
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem*> FeatureMap; // maps conceptual feature vertices to their graphical node in the scene
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem::id_t> FeatureIDMap; // maps conceptual feature vertices to their feature ID

    /// empty constructor
    GraphWidget(const QString& widgetName, QWidget *parent = 0);

    /**
     * \brief constructor
     * \param mainWindow main qt application window
     * \param parent qt parent widget
     */
    GraphWidget(QMainWindow *mainWindow, QWidget *parent = 0);

    /// destructor
    virtual ~GraphWidget();

    virtual QString getClassName() const { return "graph_analysis::gui::GraphWidget"; }

    /// getter method for retrieving the note scene items map
    NodeItemMap& nodeItemMap() { return mNodeItemMap; }
    /// getter method for retrieving the edge scene items map
    EdgeItemMap& edgeItemMap() { return mEdgeItemMap; }

    /// getter method for retrieving the underlying base graph
    graph_analysis::BaseGraph::Ptr graph() { return mpGraph; }
    /// setter method for updating the underlying base graph
    void setGraph(const graph_analysis::BaseGraph::Ptr& graph) { mpGraph = graph; }

    void setGraphWidgetManager(GraphWidgetManager* graphWidgetManager) { mpGraphWidgetManager = graphWidgetManager; }
    GraphWidgetManager* getGraphWidgetManager() const;

    // virtual methods
    virtual void reset(bool keepData = false);

    virtual void clear();
    void clearWithDialog();
    virtual void resetLayoutingGraph();

    /**
     * Update the current view / filtered subgraph
     */
    virtual void updateView();
    void updateFilterView();
    void updateLayoutView();

    /**
     * Trigger the layouting of the graph widget
     */
    virtual void updateLayout() { throw std::runtime_error("graph_analysis::gui::GraphWidget::updateLayout: not implemented"); }

    /// setter method for updating the node filters
    void setNodeFilters(std::vector< graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr > nodeFilters);
    /// setter method for updating the edge filters
    void setEdgeFilters(std::vector< graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr > edgeFilters);

    /// setter method for updating the scaling factor
    void    setScaleFactor (double scaleFactor) { mScaleFactor = scaleFactor; }
    /// getter method for retrieving the scaling factor
    double  getScaleFactor () const { return mScaleFactor; }

    /// setter method - lets the graph widget know which vertex is currently being hovered over
    void setSelectedVertex(graph_analysis::Vertex::Ptr selectedVertex) { mpSelectedVertex = selectedVertex; }
    /// getter method - retrieves the vertex this graph widget knows was last being hovered over
    graph_analysis::Vertex::Ptr getSelectedVertex() { return mpSelectedVertex; }

    /// setter method - lets the graph widget know a vertex is currently being hovered over
    void setVertexSelected (bool selected) { mVertexSelected = selected; }
    /// getter method - indicates whether this graph widget knows of a vertex currently being hovered over
    bool getVertexSelected () { return mVertexSelected; }

    /// setter method - lets the graph widget know which edge is currently being hovered over
    void setSelectedEdge(graph_analysis::Edge::Ptr selectedEdge) { mpSelectedEdge = selectedEdge; }
    /// getter method - retrieves the edge this graph widget knows was last being hovered over
    graph_analysis::Edge::Ptr getSelectedEdge() { return mpSelectedEdge; }

    /// setter method - lets the graph widget know an edge is currently being hovered over
    void setEdgeSelected (bool selected) { mEdgeSelected = selected; }
    /// getter method - indicates whether this graph widget knows of an edge currently being hovered over
    bool getEdgeSelected () { return mEdgeSelected; }

    // virtual methods
    virtual void setVertexFocused(bool focused) { throw std::runtime_error("graph_analysis::gui::GraphWidget::setVertexFocused is not reimplemented");  }
    virtual void setEdgeFocused  (bool focused) { throw std::runtime_error("graph_analysis::gui::GraphWidget::setEdgeFocused is not reimplemented");    }
    virtual void clearEdgeFocus() { throw std::runtime_error("graph_analysis::gui::GraphWidget::clearEdgeFocus is not reimplemented"); }
    virtual void clearNodeFocus() { throw std::runtime_error("graph_analysis::gui::GraphWidget::clearNodeFocus is not reimplemented"); }
    virtual void setStartVertex (graph_analysis::Vertex::Ptr, NodeItem::id_t) { throw std::runtime_error("graph_analysis::gui::GraphWidget::setStartVertex is not reimplemented"); }
    virtual void setEndVertex   (graph_analysis::Vertex::Ptr, NodeItem::id_t) { throw std::runtime_error("graph_analysis::gui::GraphWidget::setEndVertex is not reimplemented");   }
    virtual void setFocusedEdge     (graph_analysis::Edge::Ptr)     { throw std::runtime_error("graph_analysis::gui::GraphWidget::setFocusedEdge is not reimplemented");   }
    virtual void setFocusedVertex   (graph_analysis::Vertex::Ptr)   { throw std::runtime_error("graph_analysis::gui::GraphWidget::setFocusedVertex is not reimplemented"); }
    virtual void syncEdgeItemMap    (graph_analysis::Edge::Ptr)    { throw std::runtime_error("graph_analysis::gui::GraphWidget::syncEdgeItemMap is not reimplemented");   }
    virtual void itemMoved() { throw std::runtime_error("graph_analysis::gui::GraphWidget::itemMoved is not reimplemented"); }
    virtual void refreshLayerViewWidget(bool) { throw std::runtime_error("graph_analysis::gui::GraphWidget::refreshLayerViewWidget is not reimplemented"); }

    /// \param msg Message in the statusbar
    /// \param time Number of milliseconds the message will be held on screen
    virtual void updateStatus(const std::string& msg, int timeInMS = 10) const;

    virtual void shuffle() { throw std::runtime_error("graph_analysis::gui::GraphWidget::shuffle is not reimplemented"); }

    virtual void setGraphLayout(const QString& layoutName);
    virtual void refresh(bool all = true);

    virtual QStringList getSupportedLayouts() const;

protected:
    QString mWidgetName;

    /// conceptual underlying graph
    graph_analysis::BaseGraph::Ptr mpGraph;

    /// Layouting
    io::GVGraph* mpGVGraph;
    graph_analysis::BaseGraph::Ptr mpLayoutingGraph;

    /// max height of the nodes in the scene (relevant for GraphViz runtime layouting)
    qreal mMaxNodeHeight;
    /// max width of the nodes in the scene (relevant for GraphViz runtime layouting)
    qreal mMaxNodeWidth;

    // Supports filtering functionality
    GraphView mGraphView;
    SubGraph::Ptr mpSubGraph;

    /// boolean witness of filtering: true when filtering has already been set/initialized; false otherwise
    bool mFiltered;

    // Mapping with data model
    // Allow mapping of semantically feature vertices to their (cluster) nodes in the scene
    FeatureMap mFeatureMap;
    // stores features IDs given their source vertices
    FeatureIDMap mFeatureIDMap;
    // Allow mapping from graph vertices to nodes in the scene
    NodeItemMap mNodeItemMap;
    // Allow mapping from graph edges to edges in the scene
    EdgeItemMap mEdgeItemMap;
    // Allow mapping from default graph edges (edges not part of the model but used to layou the GUI graph) to their main graph edges correspondents (mainly needed for labels updating)
    EdgeMap mEdgeMap;

    int mTimerId;
    /// |mScaleFactor| > 1.0 makes edges longer; otherwise, it makes them shorter | if negative, it rotates the graph 180 degrees
    double mScaleFactor;

    /// layouting engine to be used on the next layouting
    QString mLayout;

    /// vertex filters
    graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr mpVertexFilter;
    /// edge filters
    graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr mpEdgeFilter;

    /// latest selected vertex (hovered over)
    graph_analysis::Vertex::Ptr mpSelectedVertex;
    /// latest selected edge (hovered over)
    graph_analysis::Edge::Ptr mpSelectedEdge;

    /// boolean witness for a vertex being hovered over: true when a vertex is currently being hovered over; false otherwise
    bool mVertexSelected;
    /// boolean witness for an edge being hovered over: true when an edge is currently being hovered over; false otherwise
    bool mEdgeSelected;

    GraphWidgetManager* mpGraphWidgetManager;

    void gvRender(const std::string& filename);

    virtual void keyPressEvent(QKeyEvent*);

    virtual void mousePressEvent(QMouseEvent*) { throw std::runtime_error("graph_analysis::gui::GraphWidget::mousePressEvent is not reimplemented"); }
    virtual void mouseReleaseEvent(QMouseEvent*) { throw std::runtime_error("graph_analysis::gui::GraphWidget::mouseReleaseEvent is not reimplemented"); }


};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
