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

#include "NodeItem.hpp"

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

/**
 * \class GraphWidget
 * \file GraphWidget.hpp
 * \brief graph view widget interface
 */
class GraphWidget : public QGraphicsView
{
public:
    typedef std::map<graph_analysis::Edge::Ptr, graph_analysis::Edge::Ptr> EdgeMap; // meant to map default edges to their correspondent initial edges in the base graph
    typedef std::map<graph_analysis::Edge::Ptr, EdgeItem*> EdgeItemMap; // maps conceptual edges to the graphical edges in the scene
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem*> NodeItemMap; // maps conceptual cluster vertices to graphical nodes in the scene
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem*> FeatureMap; // maps conceptual feature vertices to their graphical node in the scene
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem::id_t> FeatureIDMap; // maps conceptual feature vertices to their feature ID
    typedef std::map <std::string, io::Writer*> WriterMap; // maps specific keywords to graph exporters
    typedef std::map <std::string, io::Reader*> ReaderMap; // maps specific keywords to graph importers
    typedef std::map <std::string, QIcon> IconMap; // maps specific keywords to qt loaded icons

    /// empty constructor
    GraphWidget(QWidget *parent = 0);
    /**
     * \brief constructor
     * \param graph underlying base graph
     * \param parent qt parent widget
     */
    GraphWidget(graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::Ptr( new gl::DirectedGraph() ), QWidget *parent = 0);
    /**
     * \brief constructor
     * \param mainWindow main qt application window
     * \param parent qt parent widget
     */
    GraphWidget(QMainWindow *mainWindow, QWidget *parent = 0);
    /// destructor
    ~GraphWidget();

    /// getter method for retrieving the note scene items map
    NodeItemMap& nodeItemMap() { return mNodeItemMap; }
    /// getter method for retrieving the edge scene items map
    EdgeItemMap& edgeItemMap() { return mEdgeItemMap; }

    /// getter method for retrieving the underlying base graph
    graph_analysis::BaseGraph::Ptr graph() { return mpGraph; }
    /// setter method for updating the underlying base graph
    void setGraph(graph_analysis::BaseGraph::Ptr graph);

    // pure virtual methods
    virtual void reset(bool keepData = false) = 0;
    virtual void clear() = 0;
    virtual void updateFromGraph() = 0; // NOTE: one of the filters setters has to be called in beforehand in order to perform filtering within this call

    /// setter method for updating the node filters
    void setNodeFilters(std::vector< graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr > nodeFilters);
    /// setter method for updating the edge filters
    void setEdgeFilters(std::vector< graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr > edgeFilters);

    /// setter method for updating the scaling factor
    void    setScaleFactor (double scaleFactor) { mScaleFactor = scaleFactor; }
    /// getter method for retrieving the scaling factor
    double  getScaleFactor () const { return mScaleFactor; }

    /// setter method - lets the graph widget know which vertex is currently being hovered over
    void setSelectedVertex(graph_analysis::Vertex::Ptr selectedVertex) { mpSelectedVertex = selectedVertex; }
    /// getter method - retrieves the vertex this graph widget knows was last being hovered over
    graph_analysis::Vertex::Ptr getSelectedVertex() { return mpSelectedVertex; }

    /// setter method - lets the graph widget know a vertex is currently being hovered over
    void setVertexSelected (bool selected) { mVertexSelected = selected; }
    /// getter method - indicates whether this graph widget knows of a vertex currently being hovered over
    bool getVertexSelected () { return mVertexSelected; }

    /// setter method - lets the graph widget know which edge is currently being hovered over
    void setSelectedEdge(graph_analysis::Edge::Ptr selectedEdge) { mpSelectedEdge = selectedEdge; }
    /// getter method - retrieves the edge this graph widget knows was last being hovered over
    graph_analysis::Edge::Ptr getSelectedEdge() { return mpSelectedEdge; }

    /// setter method - lets the graph widget know an edge is currently being hovered over
    void setEdgeSelected (bool selected) { mEdgeSelected = selected; }
    /// getter method - indicates whether this graph widget knows of an edge currently being hovered over
    bool getEdgeSelected () { return mEdgeSelected; }

    // virtual methods
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
    virtual void refreshLayersWidget(bool) { throw std::runtime_error("graph_analysis::gui::GraphWidget::refreshLayerWidget is not reimplemented"); }
    virtual void updateStatus(const QString&, int) { throw std::runtime_error("graph_analysis::gui::GraphWidget::updateStatus is not reimplemented"); }

protected:

    virtual void keyPressEvent(QKeyEvent*) { throw std::runtime_error("graph_analysis::gui::GraphWidget::keyPressEvent is not reimplemented"); }
    virtual void mousePressEvent(QMouseEvent*) { throw std::runtime_error("graph_analysis::gui::GraphWidget::mousePressEvent is not reimplemented"); }
    virtual void mouseReleaseEvent(QMouseEvent*) { throw std::runtime_error("graph_analysis::gui::GraphWidget::mouseReleaseEvent is not reimplemented"); }

protected:

    /// conceptual underlying graph
    graph_analysis::BaseGraph::Ptr mpGraph;

    /// io components
    /// layouting
    io::GVGraph* mpGVGraph;
    // Supports filtering functionality
    GraphView mGraphView;
    SubGraph::Ptr mpSubGraph;

    /// boolean witness of filtering: true when filtering has already been set/initialized; false otherwise
    bool mFiltered;

    // Mapping with data model
    // Allow mapping of semantically feature vertices to their (cluster) nodes in the scene
    FeatureMap mFeatureMap;
    // stores features IDs given their source vertices
    FeatureIDMap mFeatureIDMap;
    // Allow mapping from graph vertices to nodes in the scene
    NodeItemMap mNodeItemMap;
    // Allow mapping from graph edges to edges in the scene
    EdgeItemMap mEdgeItemMap;
    // Allow mapping from default graph edges (edges not part of the model but used to layou the GUI graph) to their main graph edges correspondents (mainly needed for labels updating)
    EdgeMap mEdgeMap;

    int mTimerId;
    /// |mScaleFactor| > 1.0 makes edges longer; otherwise, it makes them shorter | if negative, it rotates the graph 180 degrees
    double mScaleFactor;

    /// layouting engine to be used on the next layouting
    QString mLayout;

    /// vertex filters
    graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr mpVertexFilter;
    /// edge filters
    graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr mpEdgeFilter;

    /// latest selected vertex (hovered over)
    graph_analysis::Vertex::Ptr mpSelectedVertex;
    /// latest selected edge (hovered over)
    graph_analysis::Edge::Ptr mpSelectedEdge;

    /// boolean witness for a vertex being hovered over: true when a vertex is currently being hovered over; false otherwise
    bool mVertexSelected;
    /// boolean witness for an edge being hovered over: true when an edge is currently being hovered over; false otherwise
    bool mEdgeSelected;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
