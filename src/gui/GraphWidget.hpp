#ifndef GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
#define GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H

#include "../Graph.hpp"

#include "VertexItemBase.hpp"
#include "EdgeItemBase.hpp"

#include <QGraphicsView>

namespace graph_analysis {
namespace gui {

class Player;
class PlayerConfiguration;

/**
 * @brief Base-class for visual representations of a "Graph" in this framework
 *
 * A Qt-Widget using a qgraphicsview as a canvas. this class does not draw
 * anything yet, but provides the infrastructure for handling with a graph. is
 * intended to obtain a shared pointer to a graph which is stored elsewhere.
 * this allows multiple independent visualizations of the same basegraph.
 *
 */
class GraphWidget : public QGraphicsView
{
    Q_OBJECT
    friend class Player;

public:

    /** constructor */
    GraphWidget(const graph_analysis::BaseGraph::Ptr& graph, QWidget *parent = NULL);
    /** destructor */
    virtual ~GraphWidget();

    virtual QString getClassName() const { return "graph_analysis::gui::GraphWidget"; }

    /**
     * a mapping from an "Edge" in the graph to a responsible visualizing
     * "Item" on the scene.
     */
    typedef std::map<const graph_analysis::Edge::Ptr, EdgeItemBase*>
        EdgeItemMap;
    /**
     * a mapping from an "Vertex" in the graph to a responsible visualizing
     * "Item" on the scene.
     */
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
     * to be called by a VertexItem when its position on the canvas changed.
     */
    void vertexPositionHasChanged(VertexItemBase* item);
    /**
     * this is used to store where a Vertex element had its responsible
     * VertexItem positioned on the scene.  can be queried after a
     * "clearVisualization()" to reposition newly created items in the same
     * canvas location as before.
     *
     * cannot store the pointer to a VertexItem directly as this pointer will
     * change after repopulation of the canvas.
     */
    typedef std::map<const graph_analysis::Vertex::Ptr, QPointF>
        VertexItemCoordinateCache;
    /**
     * when a vertex changed its position, it can be stored
     *
     * allows restoring the layout after automatic re-population using.
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
    virtual void setGraph(const graph_analysis::BaseGraph::Ptr& graph);

    /**
     * Update the current view
     */
    virtual void update();

    /**
     * Current layout in item coordinates
     */
    VertexItemCoordinateCache getCurrentLayout() const;

    /**
     * Cache the current view
     */
    void cacheCurrentLayout();

    void applyCachedLayout();

    /**
     * Apply the given layout (via coordinates)
     * \return the actually applied layout, i.e. which does not contain already
     * removed vertices
     */
    VertexItemCoordinateCache applyLayout(const VertexItemCoordinateCache& coordinates);

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

    /**
     * Display a message status in the status bar
     * \param msg Message in the statusbar
     */
    virtual void updateStatus(const std::string& msg, int timeInMS = 10);

    /**
     * Play the incremental build up / or deconstruction of a graph
     * \param conf PlayerConfiguration
     */
    void play(const PlayerConfiguration& conf);

    /**
     * Call a dialog to save the current scene
     */
    void saveScene();

public slots:
    virtual void shuffle();
    virtual void refresh(bool all = true);
    virtual void updateVisualization();

    void setVertexVisible(int id, bool show);
    void setEdgeVisible(int id, bool show);

    void activateViewportMargins(int areaFlag);
    void setTopMarginHeight(int width);
    void setBottomMarginHeight(int width);
    void setLeftMarginWidth(int width);
    void setRightMarginWidth(int width);

signals:
    void currentStatus(QString, int);

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

    // Flag
    int mWithViewportMargins;
    int mLeftMarginWidth;
    int mRightMarginWidth;
    int mTopMarginHeight;
    int mBottomMarginHeight;
    QWidget* mpLeftMarginArea;
    QWidget* mpRightMarginArea;
    QWidget* mpTopMarginArea;
    QWidget* mpBottomMarginArea;

    void keyPressEvent(QKeyEvent *);

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent* event);

    EdgeItemMap mEdgeItemMap;
    VertexItemMap mVertexItemMap;
    VertexItemCoordinateCache mItemCoordinateCache;

    /**
     * Compute the horizontal scaling factor, typically same as the vertical
     * scaling factor for this widget
     */
    qreal horizonalScalingFactor() const { return transform().m11(); }
    /**
     * Compute the vertical scaling factor, typically same as the horizontal
     * scaling factor for this widget
     */
    qreal verticalScalingFactor() const { return transform().m22(); }

    /**
     * Allow to compute the margins if needed
     */
    virtual void resizeEvent(QResizeEvent* event);

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
