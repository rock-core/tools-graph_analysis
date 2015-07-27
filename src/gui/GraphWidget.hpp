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
#include <graph_analysis/lemon/Graph.hpp>

// StatusBar standard timeout (in ms)
#define DEFAULT_TIMEOUT 6900

namespace gl = graph_analysis::lemon;
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

namespace gl = graph_analysis::lemon;
namespace gui {

class NodeItem;
class EdgeItem;
class AddNodeDialog;
class ActionCommander;
class PropertyDialog;
class LayerWidget;

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
class GraphWidget : public QGraphicsView
{
public:
    typedef std::map<graph_analysis::Edge::Ptr, graph_analysis::Edge::Ptr> EdgeMap;
    typedef std::map<graph_analysis::Edge::Ptr, EdgeItem*> EdgeItemMap;
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem*> NodeItemMap;
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem*> PortMap;
    typedef std::map<graph_analysis::Vertex::Ptr, int> PortIDMap;
    typedef std::map <std::string, io::Writer*> WriterMap;
    typedef std::map <std::string, io::Reader*> ReaderMap;
    typedef std::map <std::string, QIcon> IconMap;

    GraphWidget(QWidget *parent = 0);
    GraphWidget(graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::Ptr( new gl::DirectedGraph() ), QWidget *parent = 0);
    GraphWidget(QMainWindow *mainWindow, QWidget *parent = 0);
    ~GraphWidget();

    NodeItemMap& nodeItemMap() { return mNodeItemMap; }
    EdgeItemMap& edgeItemMap() { return mEdgeItemMap; }

    graph_analysis::BaseGraph::Ptr graph() { return mpGraph; }

    virtual void reset(bool keepData = false) = 0;
    virtual void clear() = 0;
    virtual void updateFromGraph() = 0; // NOTE: one of the filters setters has to be called in beforehand in order to perform filtering within this call

    void setNodeFilters(std::vector< graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr > nodeFilters);
    void setEdgeFilters(std::vector< graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr > edgeFilters);

    void    setScaleFactor (double scaleFactor) { mScaleFactor = scaleFactor; }
    double  getScaleFactor () const { return mScaleFactor; }

    void setSelectedVertex(graph_analysis::Vertex::Ptr selectedVertex) { mpSelectedVertex = selectedVertex; }
    graph_analysis::Vertex::Ptr getSelectedVertex() { return mpSelectedVertex; }

    void setVertexSelected (bool selected) { mVertexSelected = selected; }
    bool getVertexSelected () { return mVertexSelected; }

    void setSelectedEdge(graph_analysis::Edge::Ptr selectedEdge) { mpSelectedEdge = selectedEdge; }
    graph_analysis::Edge::Ptr getSelectedEdge() { return mpSelectedEdge; }

    void setEdgeSelected (bool selected) { mEdgeSelected = selected; }
    bool getEdgeSelected () { return mEdgeSelected; }

    bool getDragDrop() { return mDragDrop; }

    virtual void setVertexFocused(bool focused) { throw std::runtime_error("graph_analysis::gui::GraphWidget::setVertexFocused is not reimplemented");  }
    virtual void setEdgeFocused  (bool focused) { throw std::runtime_error("graph_analysis::gui::GraphWidget::setEdgeFocused is not reimplemented");    }
    virtual void clearEdgeFocus() { throw std::runtime_error("graph_analysis::gui::GraphWidget::clearEdgeFocus is not reimplemented"); }
    virtual void clearNodeFocus() { throw std::runtime_error("graph_analysis::gui::GraphWidget::clearNodeFocus is not reimplemented"); }
    virtual void setStartVertex (graph_analysis::Vertex::Ptr, int) { throw std::runtime_error("graph_analysis::gui::GraphWidget::setStartVertex is not reimplemented"); }
    virtual void setEndVertex   (graph_analysis::Vertex::Ptr, int) { throw std::runtime_error("graph_analysis::gui::GraphWidget::setEndVertex is not reimplemented");   }
    virtual void setFocusedEdge     (graph_analysis::Edge::Ptr)     { throw std::runtime_error("graph_analysis::gui::GraphWidget::setFocusedEdge is not reimplemented");   }
    virtual void setFocusedVertex   (graph_analysis::Vertex::Ptr)   { throw std::runtime_error("graph_analysis::gui::GraphWidget::setFocusedVertex is not reimplemented"); }
    virtual void syncEdgeItemMap    (graph_analysis::Edge::Ptr)    { throw std::runtime_error("graph_analysis::gui::GraphWidget::syncEdgeItemMap is not reimplemented");   }
    virtual void itemMoved() { throw std::runtime_error("graph_analysis::gui::GraphWidget::itemMoved is not reimplemented"); }

protected:

    virtual void keyPressEvent(QKeyEvent*) { throw std::runtime_error("graph_analysis::gui::GraphWidget::keyPressEvent is not reimplemented"); }
    virtual void mousePressEvent(QMouseEvent*) { throw std::runtime_error("graph_analysis::gui::GraphWidget::mousePressEvent is not reimplemented"); }
    virtual void mouseReleaseEvent(QMouseEvent*) { throw std::runtime_error("graph_analysis::gui::GraphWidget::mouseReleaseEvent is not reimplemented"); }

protected:

    graph_analysis::BaseGraph::Ptr mpGraph;

    /// io components
    /// layouting
    io::GVGraph* mpGVGraph;
    // Supports filtering functionality
    GraphView mGraphView;
    SubGraph::Ptr mpSubGraph;
    bool mFiltered;

    // Mapping with data model
    // Allow mapping of semantically port vertices to their (cluster) nodes in the scene
    PortMap mPortMap;
    // stores ports IDs given their source vertices
    PortIDMap mPortIDMap;
    // Allow mapping from graph vertices to nodes in the scene
    NodeItemMap mNodeItemMap;
    // Allow mapping from graph edges to edges in the scene
    EdgeItemMap mEdgeItemMap;
    // Allow mapping from default graph edges (edges not part of the model but used to layou the GUI graph) to their main graph edges correspondents (mainly needed for labels updating)
    EdgeMap mEdgeMap;

    int mTimerId;
    /// |mScaleFactor| > 1.0 makes edges longer; otherwise, it makes them shorter | if negative, it rotates the graph 180 degrees
    double mScaleFactor;
    QString mLayout;

    graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr mpVertexFilter;
    graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr mpEdgeFilter;

    graph_analysis::Vertex::Ptr mpSelectedVertex;
    graph_analysis::Edge::Ptr mpSelectedEdge;

    bool mVertexSelected;
    bool mEdgeSelected;

    bool mDragDrop;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
