#ifndef GRAPH_ANALYSIS_GUI_COMPONENTEDITORWIDGET_H
#define GRAPH_ANALYSIS_GUI_COMPONENTEDITORWIDGET_H

#include <map>
#include <QMainWindow>
#include <QGraphicsView>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/layer_view/LayerViewWidget.hpp>

namespace graph_analysis {
namespace io {
class GVGraph;
} // end namespace io

namespace gui {

class NodeItem;
class EdgeItem;
class ActionCommander;

/**
 * \file ComponentEditorWidget.hpp
 * \class ComponentEditorWidget
 * \brief component editor graph widget implementation
 * \details maintains and displays a diagram with clusternodes, their features and the links (edges) between their ports
 */
class ComponentEditorWidget : public LayerViewWidget
{
    Q_OBJECT
public:
    /**
     * \brief constructor
     * \param parent qt parent widget
     */
    ComponentEditorWidget(QWidget *parent = 0);
    /// destructor
    virtual ~ComponentEditorWidget();

    static QString getName() { return "graph_analysis::gui::ComponentEditor"; }
    virtual QString getClassName() const { return getName(); }

    void registerTypes();

//    /**
//     * \brief introduces (creates + adds) a new vertex in the basegraph and enables it
//     * \param type requested type for the new vertex
//     * \param label requested label for the new vertex
//     * \return smart pointer to the newly created vertex instance
//     */
//    Vertex::Ptr createVertex(const std::string& type, const std::string& label = std::string());
//    /**
//     * \brief introduces (creates + adds) a new edge in the basegraph and enables it
//     * \param sourceNode requested source endpoint for the new edge
//     * \param targetNode requested target endpoint for the new edge
//     * \param label requested label for the new edge
//     * \return smart pointer to the newly created vertex instance
//     */
//    Edge::Ptr createEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label = std::string());

    /// enables the given vertex in the base graph
    void enableVertex (graph_analysis::Vertex::Ptr vertex);
    /// disables the given vertex in the base graph
    void disableVertex(graph_analysis::Vertex::Ptr vertex);
    /// enables the given edge in the base graph
    void enableEdge (graph_analysis::Edge::Ptr edge);
    /// disables the given edge in the base graph
    void disableEdge(graph_analysis::Edge::Ptr edge);

    /// respawns all graphical elements by the underlying base graph
    void itemMoved();

    /// forcefully focuses out of any edge and/or node that might be currently focused on
    void clearFocus();

    /// synchronizes out the given edge from mEdgeItemMap (i.e. deletes the edge)
    void syncEdgeItemMap(graph_analysis::Edge::Ptr concernedEdge);

    virtual void updateLayout();

public slots:
    /// shuffles all the nodes in the diagram graph editor
    void shuffle();
    /// zooms-into the diagram editor graph scene
    void zoomIn();
    /// zooms-out of the diagram editor graph scene
    void zoomOut();

    void addFeatureDialog();

    /// displays context menu (upon a right click in the scene)
    void showContextMenu(const QPoint& pos);

protected:
    /// refreshes the (read-only) layers graph editor in the omologuous widget mpLayerViewWidget; when status is false, this takes place quiently (no updates are made on the status bar)
    //void refreshLayerViewWidget(bool status = true);
    /// updates the (read-only) layers graph editor in the omologuous widget mpLayerViewWidget
    //void updateLayerViewWidget();

    /// qt mouse press callback
    void mousePressEvent(QMouseEvent* event);
    /// qt mouse release callback
    void mouseReleaseEvent(QMouseEvent* event);
    /// qt mouse double click callback
    void mouseDoubleClickEvent(QMouseEvent* event);
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
    NodeItem* mpLastFocusedNodeItem;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_COMPONENTEDITORWIDGET_H
