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

#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>

namespace omviz {

class NodeItem;
class EdgeItem;
class GVGraph;

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

    typedef std::map<std::string, NodeItem*> GVNodeItemMap;
    typedef std::map<std::string, EdgeItem*> GVEdgeItemMap;

    GraphWidget(QWidget *parent = 0);

    void addVertex(graph_analysis::Vertex::Ptr vertex);
    void addEdge(graph_analysis::Edge::Ptr edge);

    NodeItemMap& nodeItemMap() { return mNodeItemMap; }
    EdgeItemMap& edgeItemMap() { return mEdgeItemMap; }

    ::graph_analysis::BaseGraph* graph() { return mpGraph; }

    void reset(bool keepData = false);
    void clear();
    void updateFromGraph();
    void itemMoved();

    int addNodeFilter(graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr nodeFilter);
    int addEdgeFilter(graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr edgeFilter);

    void removeNodeFilter(int position);
    void removeEdgeFilter(int position);

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();

    void setLayout(QString layoutName);

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
#endif
    void drawBackground(QPainter *painter, const QRectF &rect);

    void scaleView(qreal scaleFactor);

private:
    graph_analysis::BaseGraph* mpGraph;

    GVGraph* mpGVGraph;
    // Mapping with layout engine
    GVNodeItemMap mGVNodeItemMap;
    GVEdgeItemMap mGVEdgeItemMap;

    // Mapping with data model
    // Allow mapping from graph vertexes to nodes in the scene
    NodeItemMap mNodeItemMap;
    // Allow mapping from graph edges to edges in the scene
    EdgeItemMap mEdgeItemMap;

    int mTimerId;
    QString mLayout;

    graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr mpVertexFilter;
    graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr mpEdgeFilter;
};

} // end namespace omviz
#endif // GRAPHWIDGET_H
