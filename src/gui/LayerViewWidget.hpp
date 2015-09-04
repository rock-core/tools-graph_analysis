#ifndef GRAPH_ANALYSIS_GUI_LAYERVIEWWIDGET_H
#define GRAPH_ANALYSIS_GUI_LAYERVIEWWIDGET_H

#include <QGraphicsView>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis {
namespace gui {

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
    LayerViewWidget(QWidget *parent = 0);
    /// destructor
    virtual ~LayerViewWidget();

    static QString getName() { return "graph_analysis::gui::LayerView"; }
    virtual QString getClassName() const { return getName(); }

    /// deletes all internal information; disregards keepData as the underlying base graph is treated as read-only (except by its dedicated setter method)

    /// respawns all graphical elements by the underlying base graph
    /// and trigger the layouting
    virtual void updateLayout();
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

    void resetLayoutingGraph();

private:
    /// features layer toggle: when true, feature nodes get displayed; false when they get hidden
    bool mFeatureLayerToggle;
    /// clusters layer toggle: when true, cluster nodes get displayed; false when they get hidden
    bool mClusterLayerToggle;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_LAYERVIEWWIDGET_H
