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

class ViewWidget : public GraphWidget
{
    Q_OBJECT
public:
    typedef std::map<graph_analysis::Edge::Ptr, graph_analysis::Edge::Ptr> EdgeMap;
    typedef std::map<graph_analysis::Edge::Ptr, EdgeItem*> EdgeItemMap;
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem*> NodeItemMap;
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem*> PortMap;
    typedef std::map<graph_analysis::Vertex::Ptr, int> PortIDMap;
    typedef std::map <std::string, io::Writer*> WriterMap;
    typedef std::map <std::string, io::Reader*> ReaderMap;
    typedef std::map <std::string, QIcon> IconMap;

    ViewWidget(QMainWindow *mainWindow, QWidget *parent = 0);
    ~ViewWidget();

    Vertex::Ptr createVertex(const std::string& type, const std::string& label = std::string());
    Edge::Ptr createEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label = std::string());
    Vertex::Ptr createStandaloneVertex(const std::string& type, const std::string& label = std::string());
    Edge::Ptr createStandaloneEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label = std::string());
    void    addVertex(graph_analysis::Vertex::Ptr vertex);
    void removeVertex(graph_analysis::Vertex::Ptr vertex);
    void    addEdge(graph_analysis::Edge::Ptr edge);
    void removeEdge(graph_analysis::Edge::Ptr edge);
    void toYmlFile(const std::string& filename);
    void gvRender (const std::string& filename);
    void toDotFile(const std::string& filename);
    void toXmlFile(const std::string& filename);
    void fromXmlFile(const std::string& filename);
    void fromYmlFile(const std::string& filename);

    void enableVertex (graph_analysis::Vertex::Ptr vertex);
    void disableVertex(graph_analysis::Vertex::Ptr vertex);
    void enableEdge (graph_analysis::Edge::Ptr edge);
    void disableEdge(graph_analysis::Edge::Ptr edge);

    graph_analysis::BaseGraph::Ptr graph() { return mpGraph; }

    void reset(bool keepData = false);
    void clear();
    void updateFromGraph(); // NOTE: one of the filters setters has to be called in beforehand in order to perform filtering within this call
    void itemMoved();

    void setFocusedVertex(graph_analysis::Vertex::Ptr focusedVertex) { mpFocusedVertex = focusedVertex; }
    graph_analysis::Vertex::Ptr getFocusedVertex() { return mpFocusedVertex; }

    void setFocusedEdge(graph_analysis::Edge::Ptr focusedEdge) { mpFocusedEdge = focusedEdge; }
    graph_analysis::Edge::Ptr getFocusedEdge() { return mpFocusedEdge; }

    void setVertexFocused (bool focused);
    bool getVertexFocused () { return mVertexFocused; }

    void setEdgeFocused (bool focused);
    bool getEdgeFocused () { return mEdgeFocused; }

    LayerWidget* getLayerWidget() { return mpLayerWidget; }

    void setStartVertex (graph_analysis::Vertex::Ptr startVertex,   int portID);
    void setEndVertex   (graph_analysis::Vertex::Ptr endVertex,     int portID);

    QIcon* getIcon(std::string key) { return &(mIconMap[key]); }
    void clearNodeFocus();
    void clearEdgeFocus();
    void clearFocus();
    void addPort(graph_analysis::Vertex::Ptr vertex);
    void changeVertexLabel(graph_analysis::Vertex::Ptr vertex, const std::string& label);
    void renamePort (graph_analysis::Vertex::Ptr concernedVertex);
    void removePort (graph_analysis::Vertex::Ptr concernedVertex);
    void removePorts(graph_analysis::Vertex::Ptr concernedVertex);
    void clearVertex(graph_analysis::Vertex::Ptr concernedVertex);
    void changeEdgeLabel(graph_analysis::Edge::Ptr concernedEdge, const std::string& label);
    void clearEdge(graph_analysis::Edge::Ptr concernedEdge);
    void syncEdgeItemMap(graph_analysis::Edge::Ptr concernedEdge);
    void swapPorts(graph_analysis::Vertex::Ptr concernedVertex);

    bool dialogIsRunning();
    inline void updateStatus(const std::string& message = std::string(), int timeout = 0)
    {
        mpStatus->showMessage(QString(message.c_str()), timeout);
    }

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
    void addNodeAdhoc(QObject *pos = (QObject *) new QPoint(0, 0));
    void addPortSelected();
    void addPortFocused();
    void renamePortFocused();
    void removePortsFocused();
    void renamePortSelected();
    void removePortsSelected();
    void removePortFocused();
    void removePortSelected();
    void showContextMenu(const QPoint& pos);

    void setLayout(QString layoutName);
    void refresh();
    void changeFocusedVertexLabel();
    void changeSelectedVertexLabel();
    void resetGraph();
    void changeLayout();
    void removeSelectedVertex();
    void removeFocusedVertex();
    void changeFocusedEdgeLabel();
    void changeSelectedEdgeLabel();
    void removeFocusedEdge();
    void removeSelectedEdge();
    void swapPortsFocused();
    void swapPortsSelected();
    void importGraph();
    void exportGraph();
    void updateDragDrop(bool dragDrop);
    void syncDragDrop();
    void setDragDrop();
    void unsetDragDrop();
    void reloadPropertyDialog();
    void addEdgeAdHoc(); // assumes the concerned edge-creation member fields are properly set already

    void importGraphLayer();
    void resetGraphLayer();

    void changeFocusedEdgeLabelMainWindow();
    void removeFocusedEdgeMainWindow();
    void changeFocusedVertexLabelMainWindow();
    void removeFocusedVertexMainWindow();
    void addPortFocusedMainWindow();
    void swapPortsFocusedMainWindow();
    void renamePortFocusedMainWindow();
    void removePortFocusedMainWindow();
    void removePortsFocusedMainWindow();
    void addNodeAdhocMainWindow();
    void refreshMainWindow();
    void shuffleMainWindow();
    void changeLayoutMainWindow();
    void toggleDragDrop();
    void reloadPropertyDialogMainWindow();

protected:

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
#endif
    void drawBackground(QPainter *painter, const QRectF& rect);

    void scaleView(qreal scaleFactor);

private:
    void spawnEdge(const std::string& label); // assumes the concerned edge-creation member fields are properly set already

    QMainWindow* mpMainWindow;
    QStackedWidget* mpStackedWidget;
    graph_analysis::BaseGraph::Ptr mpLayoutingGraph;

    /// io components
    /// export
    WriterMap mWriterMap;
    /// import
    ReaderMap mReaderMap;
    graph_analysis::Vertex::Ptr mpFocusedVertex;
    graph_analysis::Vertex::Ptr mpStartVertex;
    graph_analysis::Vertex::Ptr mpStartPort;
    graph_analysis::Vertex::Ptr mpEndVertex;
    graph_analysis::Vertex::Ptr mpEndPort;
    graph_analysis::Edge::Ptr mpFocusedEdge;

    bool mVertexFocused;
    bool mEdgeFocused;
    bool mInitialized;

    PropertyDialog* mpPropertyDialog;
    qreal mMaxNodeHeight, mMaxNodeWidth;

    // icons
    IconMap mIconMap;
    void loadIcon(QIcon& icon, std::string file);

    LayerWidget* mpLayerWidget;
    QStatusBar* mpStatus;

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_VIEWWIDGET_H
