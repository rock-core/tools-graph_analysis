#ifndef GRAPH_ANALYSIS_GUI_COMPONENTEDITORWIDGET_H
#define GRAPH_ANALYSIS_GUI_COMPONENTEDITORWIDGET_H

#include "NodeItem.hpp"
#include "GraphWidget.hpp"

#include <map>
#include <QMainWindow>
#include <QGraphicsView>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/lemon/Graph.hpp>

namespace graph_analysis {
namespace io {

class Writer;
class Reader;
class GVGraph;
class YamlWriter;
class GexfWriter;
class GexfReader;
class YamlReader;
class GraphvizWriter;

} // end namespace io

namespace gui {

class NodeItem;
class EdgeItem;
class AddNodeDialog;
class ActionCommander;
class PropertyDialog;
class LayerViewWidget;

/**
 * \file ComponentEditorWidget.hpp
 * \class ComponentEditorWidget
 * \brief component editor graph widget implementation
 * \details maintains and displays a diagram with clusternodes, their features and the links (edges) between their ports
 */
class ComponentEditorWidget : public GraphWidget
{
    Q_OBJECT
public:
    /**
     * \brief constructor
     * \param parent qt parent widget
     */
    ComponentEditorWidget(GraphWidgetManager* graphWidgetManager, QWidget *parent = 0);
    /// destructor
    virtual ~ComponentEditorWidget();

    static QString getName() { return "graph_analysis::gui::ComponentEditor"; }
    virtual QString getClassName() const { return getName(); }

    /**
     * \brief introduces (creates + adds) a new vertex in the basegraph and enables it
     * \param type requested type for the new vertex
     * \param label requested label for the new vertex
     * \return smart pointer to the newly created vertex instance
     */
    Vertex::Ptr createVertex(const std::string& type, const std::string& label = std::string());
    /**
     * \brief introduces (creates + adds) a new edge in the basegraph and enables it
     * \param sourceNode requested source endpoint for the new edge
     * \param targetNode requested target endpoint for the new edge
     * \param label requested label for the new edge
     * \return smart pointer to the newly created vertex instance
     */
    Edge::Ptr createEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label = std::string());
    /**
     * \brief creates a new vertex without enabling it
     * \param type requested type for the new vertex
     * \param label requested label for the new vertex
     * \return smart pointer to the newly created vertex instance
     */
    Vertex::Ptr createStandaloneVertex(const std::string& type, const std::string& label = std::string());
    /**
     * \brief creates a new edge without enabling it
     * \param sourceNode requested source endpoint for the new edge
     * \param targetNode requested target endpoint for the new edge
     * \param label requested label for the new edge
     * \return smart pointer to the newly created vertex instance
     */
    Edge::Ptr createStandaloneEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label = std::string());
    /// adds given vertex to the base graph without enabling it
    void addVertex(graph_analysis::Vertex::Ptr vertex);
    /// removes the given vertex from the base graph
    void removeVertex(graph_analysis::Vertex::Ptr vertex);
    /// adds given edge to the base graph without enabling it
    void addEdge(graph_analysis::Edge::Ptr edge);
    /// removes the given edge from the base graph
    void removeEdge(graph_analysis::Edge::Ptr edge);

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

    /// setter method - lets this graph widget know which vertex is currently focused on (i.e. was double clicked)
    void setFocusedVertex(graph_analysis::Vertex::Ptr focusedVertex) { mpFocusedVertex = focusedVertex; }
    /// getter method - retrieves the vertex this graph widget knows was last focused on (i.e. was double clicked)
    graph_analysis::Vertex::Ptr getFocusedVertex() { return mpFocusedVertex; }

    /// setter method - lets this graph widget know which edge is currently focused on (i.e. was double clicked)
    void setFocusedEdge(graph_analysis::Edge::Ptr focusedEdge) { mpFocusedEdge = focusedEdge; }
    /// getter method - retrieves the edge this graph widget knows was last focused on (i.e. was double clicked)
    graph_analysis::Edge::Ptr getFocusedEdge() { return mpFocusedEdge; }

    /// setter method - lets this graph widget know whether a vertex is focused on (i.e. was double clicked)
    void setVertexFocused (bool focused);
    /// getter method - tells whether this graph widget knows about a vertex still being focused on
    bool getVertexFocused () { return mVertexFocused; }

    /// setter method - lets this graph widget know whether an edge is focused on (i.e. was double clicked)
    void setEdgeFocused (bool focused);
    /// getter method - tells whether this graph widget knows about an edge still being focused on
    bool getEdgeFocused () { return mEdgeFocused; }

    /// setter method - updates drag-n-drop starting vertex and the relevant source feature ID
    void setStartVertex (graph_analysis::Vertex::Ptr startVertex,   NodeItem::id_t featureID);
    /// setter method - updates drag-n-drop   ending vertex and the relevant target feature ID
    void setEndVertex   (graph_analysis::Vertex::Ptr endVertex,     NodeItem::id_t featureID);

    /// forcefully focuses out of any node that might be currently focused on
    void clearNodeFocus();
    /// forcefully focuses out of any edge that might be currently focused on
    void clearEdgeFocus();
    /// forcefully focuses out of any edge and/or node that might be currently focused on
    void clearFocus();
    /// adds a feature to the provided cluster vertex
    void addFeature(graph_analysis::Vertex::Ptr vertex);
    /// renames given vertex to the new given label 'label' (also updates the corresponding graphical representation)
    void changeVertexLabel(graph_analysis::Vertex::Ptr vertex, const std::string& label);
    /// prompts the user for renaming a feature vertex of the given cluster vertex
    void renameFeature (graph_analysis::Vertex::Ptr concernedVertex);
    /// prompts the user for removing a feature vertex of the given cluster vertex
    void removeFeature (graph_analysis::Vertex::Ptr concernedVertex);
    /// prompts the user for removing all features vertex of the given cluster vertex
    void removeFeatures(graph_analysis::Vertex::Ptr concernedVertex);
    /// prompts the user for swapping two features of the given vertex
    void swapFeatures(graph_analysis::Vertex::Ptr concernedVertex);
    /// deletes the given cluster vertex
    void clearVertex(graph_analysis::Vertex::Ptr concernedVertex);

    /// renames given edge to the new given label 'label' (also updates the corresponding graphical representation)
    void changeEdgeLabel(graph_analysis::Edge::Ptr concernedEdge, const std::string& label);
    /// deletes the given edge
    void clearEdge(graph_analysis::Edge::Ptr concernedEdge);
    /// synchronizes out the given edge from mEdgeItemMap (i.e. deletes the edge)
    void syncEdgeItemMap(graph_analysis::Edge::Ptr concernedEdge);

    /// getter method for retrieving the current mode; when true, drag-n-drop mode is active; when false, move-around mode is currently active
    bool getDragDrop() { return mDragDrop; }

    /// override existing parent methods
    void resetLayoutingGraph();

public slots:
    /// shuffles all the nodes in the diagram graph editor
    void shuffle();
    /// zooms-into the diagram editor graph scene
    void zoomIn();
    /// zooms-out of the diagram editor graph scene
    void zoomOut();
    /// prompts the used for adding a new node to the scene (the underlying graph also gets updated) optionally at a given position (e.g. mouse pointer position in the scene)
    void addNodeAdhoc(QObject *pos = (QObject *) new QPoint(0, 0));
    /// prompts the user for adding a feature to the currently selected node (i.e. node being hovered over)
    void addFeatureSelected();
    /// prompts the user for adding a feature to the currently focused node (i.e. previously double clicked node)
    void addFeatureFocused();
    /// prompts the user for renaming a feature of the currently focused node (i.e. previously double clicked node)
    void renameFeatureFocused();
    /// prompts the user for removing a feature of the currently focused node (i.e. previously double clicked node)
    void removeFeatureFocused();
    /// prompts the user for removing all features of the currently focused node (i.e. previously double clicked node)
    void removeFeaturesFocused();
    /// prompts the user for renaming a feature of the currently selected node (i.e. node being hovered over)
    void renameFeatureSelected();
    /// prompts the user for removing a feature of the currently selected node (i.e. node being hovered over)
    void removeFeatureSelected();
    /// prompts the user for removing all features of the currently selected node (i.e. node being hovered over)
    void removeFeaturesSelected();
    /// prompts the user for renaming the currently focused node (i.e. previously double clicked node)
    void changeFocusedVertexLabel();
    /// prompts the user for renaming the currently selected node (i.e. node being hovered over)
    void changeSelectedVertexLabel();
    /// deletes the currently focused node (i.e. previously double clicked node)
    void removeFocusedVertex();
    /// deletes the currently selected node (i.e. node being hovered over)
    void removeSelectedVertex();
    /// prompts the user for swapping two features of the currently focused node (i.e. previously double clicked node)
    void swapFeaturesFocused();
    /// prompts the user for swapping two features of the currently selected node (i.e. node being hovered over)
    void swapFeaturesSelected();
    /// prompts the user for renaming the currently focused edge (i.e. previously double clicked edge)
    void changeFocusedEdgeLabel();
    /// prompts the user for renaming the currently selected edge (i.e. edge being hovered over)
    void changeSelectedEdgeLabel();
    /// deletes the currently focused edge (i.e. previously double clicked edge)
    void removeFocusedEdge();
    /// deletes the currently selected edge (i.e. edge being hovered over)
    void removeSelectedEdge();

    /// displays context menu (upon a right click in the scene)
    void showContextMenu(const QPoint& pos);

    /// updates drag-n-drop witness value to 'dragDrop' and warns and adapts the other involved components
    void setDragDrop(bool dragDrop);
    /// updates drag-n-drop witness value to 'dragDrop'
    void updateDragDrop(bool dragDrop);
    /// updates move-around witness value to 'moveAround'
    void updateMoveAround(bool moveAround);
    /// toggles mDragDrop boolean witness
    void toggleDragDrop();
    /// synchronizes the drag-n-drop mode in graph editor with the state of the drag-n-drop witness in the property dialog/panel
    void syncDragDrop();
    /// sets drag-n-drop to true on all levels
    void setDragDrop();
    /// sets drag-n-drop to false on all levels
    void unsetDragDrop();
    /// spawns a new edge
    void addEdgeAdHoc(); // assumes the concerned edge-creation member fields are properly set already

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
    /// introduces and displays a new edge with the given label
    void spawnEdge(const std::string& label); // assumes the concerned edge-creation member fields are properly set already

    /// currently focused vertex (i.e. has been double-clicked)
    graph_analysis::Vertex::Ptr mpFocusedVertex;
    /// drag-n-drop source cluster vertex
    graph_analysis::Vertex::Ptr mpStartVertex;
    /// drag-n-drop source feature vertex
    graph_analysis::Vertex::Ptr mpStartFeature;
    /// drag-n-drop target cluster vertex
    graph_analysis::Vertex::Ptr mpEndVertex;
    /// drag-n-drop target feature vertex
    graph_analysis::Vertex::Ptr mpEndFeature;
    /// currently focused edge (i.e. has been double-clicked)
    graph_analysis::Edge::Ptr mpFocusedEdge;

    /// boolean witness telling whether a vertex is being focused on
    bool mVertexFocused;
    /// boolean witness telling whether an edge is being focused on
    bool mEdgeFocused;

    /// boolean witness for the drag-n-drop mode: true when drag-n-drop is on; false otherwise (i.e. when move-around mode is on)
    bool mDragDrop;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_COMPONENTEDITORWIDGET_H
