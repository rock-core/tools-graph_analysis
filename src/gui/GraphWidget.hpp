#ifndef GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
#define GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H

#include <map>
#include <QIcon>
#include <QMainWindow>
#include <QGraphicsView>
#include <QStackedWidget>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/gui/GraphWidgetManager.hpp>
#include <graph_analysis/gui/NodeItem.hpp>

namespace graph_analysis {
namespace io {
    class GVGraph;
}

namespace gui {

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

    /**
     * Triggers a reset of the underlying graph
     * via GraphWidgetManager::resetGraph
     */
    virtual void reset(bool keepData = false);

    /**
     * Clear the graph widget and scene
     */
    virtual void clear();

    /**
     * Reset the layouting graph, i.e, deletes and reinstanciates it
     */
    virtual void resetLayoutingGraph();

    /**
     * Prompt user before clearing the graph
     */
    void clearWithDialog();

    /**
     * Update the current view / filtered subgraph
     */
    virtual void update();
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


    // SELECT/ DESELECT
    void setFocusedElement(graph_analysis::GraphElement::Ptr element) { mpFocusedElement = element; }
    GraphElement::Ptr getFocusedElement() const { return mpFocusedElement; }
    void clearFocus() { mpFocusedElement = GraphElement::Ptr(); }
    bool isFocused(GraphElement::Ptr element) const { return mpFocusedElement == element; }

    void selectElement(graph_analysis::GraphElement::Ptr element);
    void unselectElement(graph_analysis::GraphElement::Ptr element);
    void clearElementSelection() { mElementSelection.clear(); }

    bool isInElementSelection(graph_analysis::GraphElement::Ptr element) { return std::find(mElementSelection.begin(), mElementSelection.end(), element) != mElementSelection.end(); }
    std::vector<graph_analysis::GraphElement::Ptr> getElementSelection() const { return mElementSelection; }

    // EDIT
    virtual Vertex::Ptr addVertex(const std::string& type, const std::string& label, QPoint* position = 0);
    void addVertexDialog(QObject* object = 0);
    virtual Edge::Ptr addEdge(const std::string& type, const std::string& label, Vertex::Ptr sourceVertex, Vertex::Ptr targetVertex);
    void addEdgeDialog(Vertex::Ptr sourceVertex, Vertex::Ptr targetVertex);

    virtual void syncEdgeItemMap    (graph_analysis::Edge::Ptr)    { throw std::runtime_error("graph_analysis::gui::GraphWidget::syncEdgeItemMap is not reimplemented");   }
    virtual void itemMoved() { throw std::runtime_error("graph_analysis::gui::GraphWidget::itemMoved is not reimplemented"); }

    /// \param msg Message in the statusbar
    /// \param time Number of milliseconds the message will be held on screen
    virtual void updateStatus(const std::string& msg, int timeInMS = 10) const;

    virtual void shuffle() { throw std::runtime_error("graph_analysis::gui::GraphWidget::shuffle is not reimplemented"); }

    virtual void setGraphLayout(const QString& layoutName);
    virtual void refresh(bool all = true);
    virtual void modeChanged(GraphWidgetManager::Mode mode) {}

    virtual QStringList getSupportedLayouts() const;
    void renameElement(GraphElement::Ptr element, const std::string& label);

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

    graph_analysis::GraphElement::Ptr mpFocusedElement;
    std::vector<graph_analysis::GraphElement::Ptr> mElementSelection;

    GraphWidgetManager* mpGraphWidgetManager;

    void gvRender(const std::string& filename);

    virtual void keyPressEvent(QKeyEvent*);

    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
