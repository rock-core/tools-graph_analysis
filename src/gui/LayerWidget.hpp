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

#ifndef GRAPH_ANALYSIS_GUI_LAYERWIDGET_H
#define GRAPH_ANALYSIS_GUI_LAYERWIDGET_H

#include "GraphWidget.hpp"

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
class ViewWidget;

/**
 *
 * \beginverbatim
    LayerWidget* widget = new LayerWidget;

    // Create vertices
    for(int i = 0; i < 1; ++i)
    {
        graph_analysis::Vertex::Ptr vertex(new graph_analysis::Vertex());
        widget->addVertex(vertex);
    }
 \endverbatim
 */
class LayerWidget : public GraphWidget
{
    Q_OBJECT
public:
    typedef std::map<graph_analysis::Edge::Ptr, EdgeItem*> EdgeItemMap;
    typedef std::map<graph_analysis::Vertex::Ptr, NodeItem*> NodeItemMap;

    LayerWidget(ViewWidget* viewWidget, graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::Ptr( new gl::DirectedGraph() ));
    ~LayerWidget();

    NodeItemMap& nodeItemMap() { return mNodeItemMap; }
    EdgeItemMap& edgeItemMap() { return mEdgeItemMap; }

    graph_analysis::BaseGraph::Ptr graph() { return mpGraph; }
    void setGraph(graph_analysis::BaseGraph::Ptr graph);

    void reset(bool keepData = false);
    void clear();
    void updateFromGraph(); // NOTE: one of the filters setters has to be called in beforehand in order to perform filtering within this call
    void itemMoved();

    void    setScaleFactor (double scaleFactor) { mScaleFactor = scaleFactor; }
    double  getScaleFactor () const { return mScaleFactor; }

    bool getPortLayerToggle     () { return mPortLayerToggle;    }
    bool getClusterLayerToggle  () { return mClusterLayerToggle; }

    inline bool toggledOut(graph_analysis::Vertex::Ptr vertex)
    {
        bool result =   (!mPortLayerToggle && "graph_analysis::PortVertex" == vertex->getClassName())
                            ||
                        (!mPortLayerToggle && "graph_analysis::InputPortVertex" == vertex->getClassName())
                            ||
                        (!mPortLayerToggle && "graph_analysis::OutputPortVertex" == vertex->getClassName())
                            ||
                        (!mClusterLayerToggle && "graph_analysis::ClusterVertex" == vertex->getClassName())
                    ;
        return result;
    }

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
    void showContextMenu(const QPoint& pos);

    void setLayout(QString layoutName);
    void refresh();
    void changeLayout();

    void togglePortLayer(bool toggle);
    void toggleClusterLayer(bool toggle);

protected:

    void setNodeFilters(std::vector< graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr > nodeFilters);
    void setEdgeFilters(std::vector< graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr > edgeFilters);

    void enableVertex(graph_analysis::Vertex::Ptr vertex);
    void enableEdge(graph_analysis::Edge::Ptr edge);

    void addVertex(graph_analysis::Vertex::Ptr vertex);
    void addEdge(graph_analysis::Edge::Ptr edge);

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
    ViewWidget* mpViewWidget;
    bool mPortLayerToggle;
    bool mClusterLayerToggle;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_LAYERWIDGET_H
