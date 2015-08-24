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

#include "ViewWidget.hpp"

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

/**
 * \file LayerWidget.hpp
 * \class LayerWidget
 * \brief layers graph view widget implementation
 * \details maintains and displays a layered view of its base graph (the latter is treated as read-only with the exception of its setter method)
 */
class LayerWidget : public GraphWidget
{
    Q_OBJECT
public:
    /**
     * \brief constructor
     * \param viewWidget managing view graph widget
     * \param graph underlying base graph
     */
    LayerWidget(ViewWidget* viewWidget, graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::Ptr( new gl::DirectedGraph() ));
    /// destructor
    ~LayerWidget();

    /// setter method for updating the underlying base graph
    void setGraph(graph_analysis::BaseGraph::Ptr graph);

    /// deletes all internal information; disregards keepData as the underlying base graph is treated as read-only (except by its dedicated setter method)
    void reset(bool keepData = false);
    /// resets all graphical scene storage elements
    void clear();
    /// respawns all graphical elements by the underlying base graph
    void updateFromGraph(); // NOTE: one of the filters setters has to be called in beforehand in order to perform filtering within this call
    void itemMoved();

    /// getter method for retrieving toggling witness of the entire features layer
    bool getFeatureLayerToggle     () { return mFeatureLayerToggle;    }
    /// getter method for retrieving toggling witness of the entire clusters layer
    bool getClusterLayerToggle  () { return mClusterLayerToggle; }

    /// method for updating the informative message in the StatusBar of the managing view widget
    inline void updateStatus(const std::string& message = std::string(), int timeout = 0)
    {
        mpViewWidget->updateStatus(message, timeout);
    }

    /// method for deciding whether a vertex is currently being toggled out of the scene by layer discrimination
    inline bool toggledOut(graph_analysis::Vertex::Ptr vertex)
    {
        bool result =   (!mFeatureLayerToggle && "graph_analysis::PortVertex" == vertex->getClassName())
                            ||
                        (!mFeatureLayerToggle && "graph_analysis::InputPortVertex" == vertex->getClassName())
                            ||
                        (!mFeatureLayerToggle && "graph_analysis::OutputPortVertex" == vertex->getClassName())
                            ||
                        (!mFeatureLayerToggle && "graph_analysis::PropertyVertex" == vertex->getClassName())
                            ||
                        (!mFeatureLayerToggle && "graph_analysis::OperationVertex" == vertex->getClassName())
                            ||
                        (!mClusterLayerToggle && "graph_analysis::ClusterVertex" == vertex->getClassName())
                    ;
        return result;
    }

public slots:
    /// shuffles all the nodes in the layers graph view
    void shuffle();
    /// zooms-into the layers graph scene
    void zoomIn();
    /// zooms-out of the layers graph scene
    void zoomOut();
    /// pulls-out the layers graph context menu on right-click
    void showContextMenu(const QPoint& pos);

    /// sets and layouts on the provided 'layoutName' the layers graph view
    void setLayout(QString layoutName);
    /// refreshes the scene from scratch - updates the Status bar iff status is set (true)
    void refresh(bool status = true);
    /// prompts the user for a new layout and performs re-layouting
    void changeLayout();

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

    /// adds a new vertex to the underlying base graph
    void addVertex(graph_analysis::Vertex::Ptr vertex);
    /// adds a new edge to the underlying base graph
    void addEdge(graph_analysis::Edge::Ptr edge);

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

private:
    /// managing view widget
    ViewWidget* mpViewWidget;
    /// features layer toggle: when true, feature nodes get displayed; false when they get hidden
    bool mFeatureLayerToggle;
    /// clusters layer toggle: when true, cluster nodes get displayed; false when they get hidden
    bool mClusterLayerToggle;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_LAYERWIDGET_H
