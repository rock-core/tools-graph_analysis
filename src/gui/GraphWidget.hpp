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

#include <QGraphicsView>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/lemon/Graph.hpp>
//#include <boost/thread/mutex.hpp> // no need to: SLOT calls are executed sequentially
    // more details @ http://doc.qt.io/qt-4.8/threads-qobject.html#signals-and-slots-across-threads

namespace gl = graph_analysis::lemon;
namespace graph_analysis {
namespace io {

class GVGraph;
class YamlWriter;

} // end namespace io

namespace gui {

class NodeItem;
class EdgeItem;
class AddNodeDialog;
class ActionCommander;
class PropertyDialog;

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
    Q_OBJECT
public:
    typedef std::map<graph_analysis::Edge::Ptr, EdgeItem*> EdgeItemMap;
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem*> NodeItemMap;

    GraphWidget(QWidget *parent = 0);
    ~GraphWidget();

    void addVertex(graph_analysis::Vertex::Ptr vertex);
    void addEdge(graph_analysis::Edge::Ptr edge);
    void toFile(const std::string& filename);
    void toDotFile(const std::string& filename);
    void toFileNoLayout(const std::string& filename);

    NodeItemMap& nodeItemMap() { return mNodeItemMap; }
    EdgeItemMap& edgeItemMap() { return mEdgeItemMap; }

    void enableVertex(graph_analysis::Vertex::Ptr vertex);
    void enableEdge(graph_analysis::Edge::Ptr edge);

    graph_analysis::BaseGraph::Ptr graph() { return mpGraph; }

    void reset(bool keepData = false);
    void clear();
    void updateFromGraph(); // NOTE: one of the filters setters has to be called in beforehand in order to perform filtering within this call
    void itemMoved();
    void standAloneLayouting();

    void setNodeFilters(std::vector< graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr > nodeFilters);
    void setEdgeFilters(std::vector< graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr > edgeFilters);

    void    setScaleFactor (double scaleFactor) { mScaleFactor = scaleFactor; }
    double  getScaleFactor () const { return mScaleFactor; }

    void setSelectedVertex(graph_analysis::Vertex::Ptr selectedVertex) { mpSelectedVertex = selectedVertex; }
    graph_analysis::Vertex::Ptr getSelectedVertex() { return mpSelectedVertex; }

    void setVertexSelected (bool selected) { mVertexSelected = selected; }
    bool getVertexSelected () { return mVertexSelected; }

    void setSelectedEdge(graph_analysis::Edge::Ptr selectedEdge) { mpSelectedEdge= selectedEdge; }
    graph_analysis::Edge::Ptr getSelectedEdge() { return mpSelectedEdge; }

    void setEdgeSelected (bool selected) { mEdgeSelected = selected; }
    bool getEdgeSelected () { return mEdgeSelected; }
    bool getDragDrop() { return mDragDrop; }
    bool getDragInitiated() { return mEdgeStartVertex; }
    graph_analysis::Vertex::Ptr getDragSource() { return mpStartVertex; }

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
    void addNodeAdhoc(QObject *pos = (QObject *) new QPoint(0, 0));
    void showContextMenu(const QPoint& pos);

    void setLayout(QString layoutName);
    void refresh();
    void changeSelectedVertexLabel();
    void startNewEdgeHere();
    void endNewEdgeHere();
    void changeLayout();
    void removeSelectedVertex();
    void changeSelectedEdgeLabel();
    void removeSelectedEdge();
    void exportGraph();
    void exportGraphToDot();
    void updateDragDrop(bool dragDrop);

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
#endif
    void drawBackground(QPainter *painter, const QRectF& rect);

    void scaleView(qreal scaleFactor);

private:
    void updateGVGraph();
    void spawnEdge(const std::string& label);

    graph_analysis::BaseGraph::Ptr mpGraph;

    io::GVGraph* mpGVGraph;
    io::YamlWriter* mpYamlWriter;
    // Supports filtering functionality
    GraphView mGraphView;
    SubGraph::Ptr mpSubGraph;
    bool mFiltered;

    // Mapping with data model
    // Allow mapping from graph vertexes to nodes in the scene
    NodeItemMap mNodeItemMap;
    // Allow mapping from graph edges to edges in the scene
    EdgeItemMap mEdgeItemMap;

    int mTimerId;
    /// if |mScaleFactor| > 1.0, it makes edges longer; it makes them shorter otherwise ||| if negative, it rotates the graph 180 degrees
    double mScaleFactor;
    QString mLayout;

    graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr mpVertexFilter;
    graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr mpEdgeFilter;

    graph_analysis::Vertex::Ptr mpSelectedVertex;
    graph_analysis::Edge::Ptr mpSelectedEdge;
    graph_analysis::Vertex::Ptr mpStartVertex;
    graph_analysis::Vertex::Ptr mpEndVertex;

    bool mVertexSelected;
    bool mEdgeSelected;
    bool mEdgeStartVertex;
    bool mEdgeEndVertex;
    bool mGVGraphDirty;

    PropertyDialog *mpPropertyDialog;
    bool mDragDrop;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GRAPHWIDGET_H
