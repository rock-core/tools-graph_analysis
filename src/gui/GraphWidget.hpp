#ifndef GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
#define GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H

#include <graph_analysis/Graph.hpp>
#include <graph_analysis/gui/GraphWidgetManager.hpp>

#include <graph_analysis/gui/VertexItemBase.hpp>
#include <graph_analysis/gui/EdgeItemBase.hpp>

#include <QGraphicsView>
#include <QMimeData>

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
 *
 * The GraphWidget is meant to provide the base-class for several possible
 * graphical representations of an underlying graph_analysis::BaseGraph datastructure.
 * as the visual representation, it can map between entities of
 * graph_analysis::GraphElement and QGraphicsItem
 *
 * ??? the base-graph is adopted via a FilterManager to the actual graph to be displayed.
 *
 */
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:

    /**
     * storing a mapping from an "Edge" in the graph to a responsible "Item" on the scene.
     */
    typedef std::map<const graph_analysis::Edge::Ptr, EdgeItemBase*>
        EdgeItemMap;
    typedef std::map<const graph_analysis::Vertex::Ptr, VertexItemBase*>
        VertexItemMap;
    /**
     * this is used to store where items for specific elements where positioned
     * on the scene. can be asked after a "clearVisualization()" to reposition
     * newly created items in the same location on the canvas.
     */
    typedef std::map<const graph_analysis::Vertex::Ptr, QPointF>
        VertexItemCoordinateCache;

    void registerEdgeItem(const graph_analysis::Edge::Ptr& e, EdgeItemBase* i);
    void registerVertexItem(const graph_analysis::Vertex::Ptr& v,
                            VertexItemBase* i);
    void deregisterEdgeItem(const graph_analysis::Edge::Ptr& e,
                            EdgeItemBase* i);
    void deregisterVertexItem(const graph_analysis::Vertex::Ptr& v,
                              VertexItemBase* i);
    EdgeItemBase* lookupEdgeItem(const graph_analysis::Edge::Ptr& e) const;
    VertexItemBase*
    lookupVertexItem(const graph_analysis::Vertex::Ptr& v) const;

    /**
     * to be called when a vertex changes its position.
     *
     * used in order to attach edges to vertices.
     */
    void cacheVertexItemPosition(const graph_analysis::Vertex::Ptr v, QPointF p);

    /**
     * \brief constructor
     * \param mainWindow main qt application window
     * \param parent qt parent widget
     */
    GraphWidget(QWidget *parent = NULL);

    /// destructor
    virtual ~GraphWidget();

    virtual QString getClassName() const { return "graph_analysis::gui::GraphWidget"; }

    /// getter method for retrieving the underlying base graph
    graph_analysis::BaseGraph::Ptr graph() const { return mpGraph; }
    /// setter method for updating the underlying base graph
    void setGraph(const graph_analysis::BaseGraph::Ptr& graph) { mpGraph = graph; }

    /**
     * Triggers a reset of the underlying graph
     * via GraphWidgetManager::resetGraph
     */
    virtual void reset();

    /**
     * Reset the layouting graph, i.e, deletes and reinstanciates it
     */
    virtual void resetLayoutingGraph();

    /**
     * Update the current view
     */
    virtual void update();
    virtual void updateView();
    void updateLayoutView();

    /**
     * Trigger the layouting of the graph widget
     */
    virtual void populateCanvas()
    {
        throw std::runtime_error(
            "graph_analysis::gui::GraphWidget::populateCanvas: not implemented");
    }

    // SELECT/ DESELECT
    void setFocusedElement(const GraphElement::Ptr& element);
    GraphElement::Ptr getFocusedElement() const { return mpFocusedElement; }
    void clearFocus();
    bool isFocused(const GraphElement::Ptr& element) const { return mpFocusedElement == element; }

    /**
     * Clear visualization and scene not(!) the underlying graph
     */
    virtual void clearVisualization();

    // \param msg Message in the statusbar
    /// \param time Number of milliseconds the message will be held on screen
    virtual void updateStatus(const std::string& msg, int timeInMS = 10);

public slots:
    virtual void shuffle();
    virtual void refresh(bool all = true);
    virtual void updateVisualization();

signals:
    void currentStatus(QString, int);
    void baseGraphChanged();

protected:

    // the QGraphicsScene to be used by this widget
    QGraphicsScene* mpScene;

    /// qt mouse wheel spin callback
    void wheelEvent(QWheelEvent *event);
    /// scales scene (zooms into or out of the scene)
    void scaleView(qreal scaleFactor);

    /// shared pointer of the conceptual, underlying graph
    graph_analysis::BaseGraph::Ptr mpGraph;

    /**
     * this is (was?) used to display the name of the last focused element in
     * the status bar.
     */
    graph_analysis::GraphElement::Ptr mpFocusedElement;

    void keyPressEvent(QKeyEvent *);

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent* event);

    EdgeItemMap mEdgeItemMap;
    VertexItemMap mVertexItemMap;
    VertexItemCoordinateCache mItemCoordinateMap;

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
