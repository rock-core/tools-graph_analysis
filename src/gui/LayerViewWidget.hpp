#ifndef GRAPH_ANALYSIS_GUI_LAYERVIEWWIDGET_H
#define GRAPH_ANALYSIS_GUI_LAYERVIEWWIDGET_H

#include "ComponentEditorWidget.hpp"

#include <QGraphicsView>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/lemon/Graph.hpp>

namespace gl = graph_analysis::lemon;
namespace graph_analysis {
namespace io {

class GVGraph;

} // end namespace io

namespace gui {

class NodeItem;
class EdgeItem;

/**
 * \file LayerViewWidget.hpp
 * \class LayerViewWidget
 * \brief layers graph view widget implementation
 * \details maintains and displays a layered view of its base graph (the latter is treated as read-only with the exception of its setter method)
 */
class LayerViewWidget : public GraphWidget
{
    Q_OBJECT
public:
    /**
     * \brief constructor
     * \param graph underlying base graph
     */
    LayerViewWidget(graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::Ptr( new gl::DirectedGraph() ), QWidget *parent = 0);
    /// destructor
    ~LayerViewWidget();

    /// setter method for updating the underlying base graph
    void setGraph(graph_analysis::BaseGraph::Ptr graph);

    /// deletes all internal information; disregards keepData as the underlying base graph is treated as read-only (except by its dedicated setter method)
    void reset(bool keepData = false);
    /// resets all graphical scene storage elements
    void clear();
    /// respawns all graphical elements by the underlying base graph
    void updateFromGraph(); // NOTE: one of the filters setters has to be called in beforehand in order to perform filtering within this call
    void itemMoved();

    /// getter method for retrieving toggling witness of the entire features layer
    bool getFeatureLayerToggle     () { return mFeatureLayerToggle;    }
    /// getter method for retrieving toggling witness of the entire clusters layer
    bool getClusterLayerToggle  () { return mClusterLayerToggle; }

    /// method for deciding whether a vertex is currently being toggled out of the scene by layer discrimination
    inline bool toggledOut(graph_analysis::Vertex::Ptr vertex);

public slots:
    /// shuffles all the nodes in the layers graph view
    void shuffle();
    /// zooms-into the layers graph scene
    void zoomIn();
    /// zooms-out of the layers graph scene
    void zoomOut();
    /// pulls-out the layers graph context menu on right-click
    void showContextMenu(const QPoint& pos);

    /// sets and layouts on the provided 'layoutName' the layers graph view
    void setLayout(QString layoutName);
    /// refreshes the scene from scratch - updates the Status bar iff status is set (true)
    void refresh(bool status = true);
    /// prompts the user for a new layout and performs re-layouting
    void changeLayout();

    /// setter method for updating the features layer toggle
    void toggleFeatureLayer(bool toggle);
    /// setter method for updating the clusters layer toggle
    void toggleClusterLayer(bool toggle);

protected:

    /// enables the provided vertex (its corresponding node scene-item gets displayed at re-rendering)
    void enableVertex(graph_analysis::Vertex::Ptr vertex);
    /// disables the provided vertex (its corresponding node scene-item gets filtered out at re-rendering)
    void disableVertex(graph_analysis::Vertex::Ptr vertex);
    /// enables the provided edge (its corresponding edge scene-item gets displayed at re-rendering)
    void enableEdge(graph_analysis::Edge::Ptr edge);
    /// enables the provided edge (its corresponding edge scene-item gets filtered out at re-rendering)
    void disableEdge(graph_analysis::Edge::Ptr edge);

    /// adds a new vertex to the underlying base graph
    void addVertex(graph_analysis::Vertex::Ptr vertex);
    /// adds a new edge to the underlying base graph
    void addEdge(graph_analysis::Edge::Ptr edge);

    /// qt mouse press callback
    void mousePressEvent(QMouseEvent *event);
    /// qt mouse release callback
    void mouseReleaseEvent(QMouseEvent *event);
    /// qt keys press callback
    void keyPressEvent(QKeyEvent *event);
    /// qt timer callback
    void timerEvent(QTimerEvent *event);
#ifndef QT_NO_WHEELEVENT
    /// qt mouse wheel spin callback
    void wheelEvent(QWheelEvent *event);
#endif
    void drawBackground(QPainter *painter, const QRectF& rect);

    /// scales scene (zooms into or out of the scene)
    void scaleView(qreal scaleFactor);

private:
    /// features layer toggle: when true, feature nodes get displayed; false when they get hidden
    bool mFeatureLayerToggle;
    /// clusters layer toggle: when true, cluster nodes get displayed; false when they get hidden
    bool mClusterLayerToggle;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_LAYERVIEWWIDGET_H
