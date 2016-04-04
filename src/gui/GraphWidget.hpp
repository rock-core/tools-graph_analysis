#ifndef GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
#define GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H

#include <graph_analysis/Graph.hpp>

#include <graph_analysis/gui/VertexItemBase.hpp>
#include <graph_analysis/gui/EdgeItemBase.hpp>

#include <QGraphicsView>

namespace graph_analysis {
namespace io {
    class GVGraph;
}

namespace gui {

/**
 *
 */
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:

    /** constructor */
    GraphWidget(graph_analysis::BaseGraph::Ptr graph, QWidget *parent = NULL);
    /** destructor */
    virtual ~GraphWidget();

    virtual QString getClassName() const { return "graph_analysis::gui::GraphWidget"; }

    /**
     * storing a mapping from an "Edge" in the graph to a responsible "Item" on the scene.
     */
    typedef std::map<const graph_analysis::Edge::Ptr, EdgeItemBase*>
        EdgeItemMap;
    typedef std::map<const graph_analysis::Vertex::Ptr, VertexItemBase*>
        VertexItemMap;

    /** to be called after a new "EdgeItem" is added to the canvas */
    void registerEdgeItem(const graph_analysis::Edge::Ptr& e, EdgeItemBase* i);
    /** to be called after a new "VertexItem" is added to the canvas */
    void registerVertexItem(const graph_analysis::Vertex::Ptr& v,
                            VertexItemBase* i);
    /** to be called after an existing "EdgeItem" is removed from the canvas */
    void deregisterEdgeItem(const graph_analysis::Edge::Ptr& e,
                            EdgeItemBase* i);
    /** to be called after an existing "VertexItem" is removed from the canvas */
    void deregisterVertexItem(const graph_analysis::Vertex::Ptr& v,
                              VertexItemBase* i);

    /**
     * takes the "connector" and the "boundingRect" of both items and tries to
     * create a line between the intersection point of the connecting line and
     * the boundingRects
     *
     * used to establish sensible coordinated to re-draw edges after their
     * vertices changed position
     */
    QVector<QPointF> getEdgePoints(VertexItemBase* firstItem,
                                   VertexItemBase* secondItem) const;

    /**
     * to be called by a VertexItem when it changes its position.
     */
    void vertexPositionHasChanged(VertexItemBase* item);
    /**
     * this is used to store where items for specific elements where positioned
     * on the scene. can be asked after a "clearVisualization()" to reposition
     * newly created items in the same location on the canvas.
     */
    typedef std::map<const graph_analysis::Vertex::Ptr, QPointF>
        VertexItemCoordinateCache;
    /**
     * when a vertex changes its position it is stored
     *
     * allows restoring the layout after automatic re-population
     */
    void cacheVertexItemPosition(const graph_analysis::Vertex::Ptr v, QPointF p);
    /**
     * update all edges associated with the given vertex after a position change
     *
     * the default implementation tries to draw a line between the intersection
     * points of the "connectors" and the "boundingRect" of each item of the edge.
     */
    virtual void updateEdgePositions(VertexItemBase* item);

    /// getter method for retrieving the underlying base graph
    graph_analysis::BaseGraph::Ptr graph() const;
    /// setter method for updating the underlying base graph
    void setGraph(const graph_analysis::BaseGraph::Ptr& graph);

    /**
     * Update the current view
     */
    virtual void update();
    virtual void updateView();
    void updateLayoutView();

    /**
     * trigger the creation of individual QGraphicsItems based on the
     * referenced BaseGraph for this instance of a graph widget.
     */
    virtual void populateCanvas()
    {
        throw std::runtime_error(
            "graph_analysis::gui::GraphWidget::populateCanvas: not implemented");
    }

    /** infrastructure for showing information about the currently hovered
     * edge/vertex in the statusbar */
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
