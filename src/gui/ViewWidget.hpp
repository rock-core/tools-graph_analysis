/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GRAPH_ANALYSIS_GUI_VIEWWIDGET_H
#define GRAPH_ANALYSIS_GUI_VIEWWIDGET_H

#include "NodeItem.hpp"
#include "GraphWidget.hpp"

#include <map>
#include <QIcon>
#include <QStatusBar>
#include <QMainWindow>
#include <QGraphicsView>
#include <QStackedWidget>
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
class LayerWidget;

/**
 * \class ViewWidget
 * \file ViewWidget.hpp
 * \brief diagram view graph widget implementation
 * \details maintains and displays a diagram with clusternodes, their features and the links (edges) between their ports
 */
class ViewWidget : public GraphWidget
{
    Q_OBJECT
public:
    /**
     * \brief constructor
     * \param mainWindow main qt application window
     * \param parent qt parent widget
     */
    ViewWidget(QMainWindow *mainWindow, QWidget *parent = 0);
    /// destructor
    ~ViewWidget();

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

    /// renders the base graph to custom yml format file 'filename'
    void toYmlFile(const std::string& filename);
    /// renders the base graph to DOT file 'filename' via mpGVGraph directly
    void gvRender (const std::string& filename);
    /// renders the base graph to DOT file 'filename' via the dot exporter (graph writer)
    void toDotFile(const std::string& filename);
    /// renders the base graph to gexf format file 'filename'
    void toXmlFile(const std::string& filename);
    /// loads base graph from gexf format file 'filename'
    void fromXmlFile(const std::string& filename);
    /// loads base graph from custo yml format file 'filename'
    void fromYmlFile(const std::string& filename);

    /// enables the given vertex in the base graph
    void enableVertex (graph_analysis::Vertex::Ptr vertex);
    /// disables the given vertex in the base graph
    void disableVertex(graph_analysis::Vertex::Ptr vertex);
    /// enables the given edge in the base graph
    void enableEdge (graph_analysis::Edge::Ptr edge);
    /// disables the given edge in the base graph
    void disableEdge(graph_analysis::Edge::Ptr edge);

    /// deletes all internal information; when keepData is set, the underlying base graph is spared (it won't get reset)
    void reset(bool keepData = false);
    /// resets all graphical scene storage elements
    void clear();
    /// respawns all graphical elements by the underlying base graph
    void updateFromGraph(); // NOTE: one of the filters setters has to be called in beforehand in order to perform filtering within this call
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

    /// getter method - retrieves available handle on the au-pair layers graph widget
    LayerWidget* getLayerWidget() { return mpLayerWidget; }

    /// setter method - updates drag-n-drop starting vertex and the relevant source feature ID
    void setStartVertex (graph_analysis::Vertex::Ptr startVertex,   int featureID);
    /// setter method - updates drag-n-drop   ending vertex and the relevant target feature ID
    void setEndVertex   (graph_analysis::Vertex::Ptr endVertex,     int featureID);

    /// getter method - retrieves a qt icon registered at the given keyword in the icons map
    QIcon* getIcon(std::string key) { return &(mIconMap[key]); }

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

    /// retrieves a boolean witness telling whether the property dialog/panel is up and running
    bool dialogIsRunning();

    /// setter method for updating the temporary text currently being displayed on the Status Bar
    inline void updateStatus(const std::string& message = std::string(), int timeout = 0)
    {
        mpStatus->showMessage(QString(message.c_str()), timeout);
    }

    /// setter method for updating the temporary text currently being displayed on the Status Bar
    void updateStatus(const QString& message = QString(), int timeout = 0)
    {
        mpStatus->showMessage(message, timeout);
    }

    /// getter method for retrieving the current mode; when true, drag-n-drop mode is active; when false, move-around mode is currently active
    bool getDragDrop() { return mDragDrop; }

    /**
     * \brief refreshes the (read-only) layers graph view in the omologuous widget mpLayerWidget (meant to be used from the outside)
     * \param status controls Status Bar interaction: when false, refreshing goes silently (no updates on the Status Bar); when true, otherwise
     */
    void refreshLayersWidget(bool status = true);

public slots:
    /// shuffles all the nodes in the diagram graph view
    void shuffle();
    /// zooms-into the diagram view graph scene
    void zoomIn();
    /// zooms-out of the diagram view graph scene
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

    /// displays context menu (upon a right click on the scene)
    void showContextMenu(const QPoint& pos);

    /// re-layouts the graph view with the given layouting engine 'layoutName'
    void setLayout(QString layoutName);
    /// refreshes the scene from scratch - updates the Status bar iff status is set (true)
    void refresh(bool status = true);
    /// deletes the entire graph
    void resetGraph();
    /// prompts the user for a new layout and performs re-layouting
    void changeLayout();
    /// prompts the user for loading the base graph from a file
    void importGraph();
    /// prompts the user for saving the base graph to a file
    void exportGraph();
    /// updates drag-n-drop witness value to 'dragDrop' and warns and adapts the other involved components
    void setDragDrop(bool dragDrop);
    /// updates drag-n-drop witness value to 'dragDrop'
    void updateDragDrop(bool dragDrop);
    /// updates move-around witness value to 'moveAround'
    void updateMoveAround(bool moveAround);
    /// toggles mDragDrop boolean witness
    void toggleDragDrop();
    /// synchronizes the drag-n-drop mode in graph view with the state of the drag-n-drop witness in the property dialog/panel
    void syncDragDrop();
    /// sets drag-n-drop to true on all levels
    void setDragDrop();
    /// sets drag-n-drop to false on all levels
    void unsetDragDrop();
    /// re-loads the property dialog/panel in the scene
    void reloadPropertyDialog();
    /// spawns a new edge
    void addEdgeAdHoc(); // assumes the concerned edge-creation member fields are properly set already

    // safety slot wrappers - triggered from the main window Menu Bar
    /// triggers changeFocusedEdgeLabel() if any edge is being focused on
    void changeFocusedEdgeLabelMainWindow();
    /// triggers removeFocusedEdge() if any edge is being focused on
    void removeFocusedEdgeMainWindow();
    /// triggers changeFocusedVertexLabel() if any node is being focused on
    void changeFocusedVertexLabelMainWindow();
    /// triggers removeFocusedVertex() if any node is being focused on
    void removeFocusedVertexMainWindow();
    /// triggers addFeatureFocused() if any node is being focused on
    void addFeatureFocusedMainWindow();
    /// triggers swapFeaturesFocused() if any node is being focused on
    void swapFeaturesFocusedMainWindow();
    /// triggers renameFeatureFocused() if any node is being focused on
    void renameFeatureFocusedMainWindow();
    /// triggers removeFeatureFocused() if any node is being focused on
    void removeFeatureFocusedMainWindow();
    /// triggers removeFeaturesFocused() if any node is being focused on
    void removeFeaturesFocusedMainWindow();
    /// triggers addNodeAdhoc() (with no mouse pointer position)
    void addNodeAdhocMainWindow();
    /// triggers refresh() in the currently displayed graph widget
    void refreshMainWindow();
    /// triggers shuffle() in the currently displayed graph widget
    void shuffleMainWindow();
    /// triggers changeLayout() in the currently displayed graph widget
    void changeLayoutMainWindow();
    /// reloads the property dialog/panel if it is not running any more
    void reloadPropertyDialogMainWindow();

protected:
    /// refreshes the (read-only) layers graph view in the omologuous widget mpLayerWidget
    inline void refreshLayerWidget();
    /// updates the (read-only) layers graph view in the omologuous widget mpLayerWidget
    inline void updateLayerWidget();

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

    /// main window of the qt application
    QMainWindow* mpMainWindow;
    /// parent stacked widget (central widget of the main window)
    QStackedWidget* mpStackedWidget;
    /// secondary base graph needed for runtime re-layouting
    graph_analysis::BaseGraph::Ptr mpLayoutingGraph;

    /// io components
    /// export
    WriterMap mWriterMap;
    /// import
    ReaderMap mReaderMap;
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
    /// boolean witness preventing status bar refresh message to interfere with the 'Ready!' message on init
    bool mInitialized;

    /// handle on the property dialog/panel
    PropertyDialog* mpPropertyDialog;

    /// max height of the nodes in the scene (relevant for GraphViz runtime layouting)
    qreal mMaxNodeHeight;
    /// max width of the nodes in the scene (relevant for GraphViz runtime layouting)
    qreal mMaxNodeWidth;

    // icons
    /// qt icons map (with string keywords identifiers)
    IconMap mIconMap;
    /// loads icon from image file 'file' to the given qt icon
    void loadIcon(QIcon& icon, std::string file);

    /// handle on the au-pair layers graph widget
    LayerWidget* mpLayerWidget;
    /// status bar
    QStatusBar* mpStatus;
    /// boolean witness for the drag-n-drop mode: true when drag-n-drop is on; false otherwise (i.e. when move-around mode is on)
    bool mDragDrop;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_VIEWWIDGET_H
