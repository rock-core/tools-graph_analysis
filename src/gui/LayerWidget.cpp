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

#include "LayerWidget.hpp"
#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "NodeTypeManager.hpp"
#include "EdgeTypeManager.hpp"
#include "ActionCommander.hpp"
#include "AddNodeDialog.hpp"

#include <set>
#include <math.h>
#include <sstream>
#include <QDir>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSignalMapper>
#include <boost/regex.hpp>
#include <base/Logging.hpp>


#include "layeritem/Resource.hpp"
#include "layeritem/edges/Simple.hpp"
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>

#include <exception>
#include <boost/foreach.hpp>
#include <base/Time.hpp>

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

LayerWidget::LayerWidget(ViewWidget* viewWidget, graph_analysis::BaseGraph::Ptr graph)
    : GraphWidget(graph)
    , mpViewWidget(viewWidget)
    , mPortLayerToggle(true)
    , mClusterLayerToggle(true)
{
    // Add seed for force layout
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    setCacheMode(CacheBackground);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Graphview"));
    // Setting up filtering
    mGraphView.setVertexFilter(mpVertexFilter);
    mGraphView.setEdgeFilter(mpEdgeFilter);
    // End of setting up filters


    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(showContextMenu(const QPoint &)));

    reset(true);
    updateFromGraph();
}

LayerWidget::~LayerWidget()
{
//    destroy();
}

void LayerWidget::showContextMenu(const QPoint& pos)
{
    ActionCommander comm(mpViewWidget);
    QMenu contextMenu(tr("Context menu"), this);

    QAction *actionRefresh = comm.addAction("Refresh", SLOT(refresh()), *(mpViewWidget->getIcon("refresh")), this);
    QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffle()), *(mpViewWidget->getIcon("shuffle")), this);
    QAction *actionImport  = comm.addAction("Import", SLOT(importGraph()), *(mpViewWidget->getIcon("import")));
    QAction *actionExport  = comm.addAction("Export", SLOT(exportGraph()), *(mpViewWidget->getIcon("export")));
    QAction *actionReset   = comm.addAction("Reset", SLOT(resetGraph()), *(mpViewWidget->getIcon("reset")));
    QAction *actionLayout  = comm.addAction("Layout", SLOT(changeLayout()), *(mpViewWidget->getIcon("layout")), this);
    QAction *actionReloadPropertyDialog = comm.addAction("Reload Command Panel", SLOT(reloadPropertyDialog()), *(mpViewWidget->getIcon("reload")));

    contextMenu.addAction(actionImport);
    contextMenu.addAction(actionExport);
    contextMenu.addSeparator();
    contextMenu.addAction(actionRefresh);
    contextMenu.addAction(actionShuffle);
    contextMenu.addAction(actionReset);
    contextMenu.addAction(actionLayout);
    if(!mpViewWidget->dialogIsRunning())
    {
        contextMenu.addSeparator();
        contextMenu.addAction(actionReloadPropertyDialog);
    }
    contextMenu.exec(mapToGlobal(pos));
}

void LayerWidget::setGraph(graph_analysis::BaseGraph::Ptr graph)
{
    mpGraph = graph;
    mpSubGraph = mGraphView.apply(mpGraph);
    mFiltered = true;

    mpSubGraph->enableAllVertices();
    mpSubGraph->enableAllEdges();
    reset(true);
    updateFromGraph();
}

void LayerWidget::changeLayout()
{
    updateStatus(std::string("changing layers graph view layout..."));
    bool ok;
    QStringList options;
    std::set<std::string> supportedLayouts = mpGVGraph->getSupportedLayouts();
//    options << tr("circo") << tr("dot") << tr("fdp") << tr("neato") << tr("osage") << tr("sfdp") << tr("twopi"); // [circo, dot, fdp, neato, osage, sfdp, twopi]
    foreach(std::string supportedLayout, supportedLayouts)
    {
        options << tr(supportedLayout.c_str());
    }
    QString layout = QInputDialog::getItem(this, tr("Input New Layout"),
                                         tr("select a layout:"), options,
                                         0, false, &ok);
    if (ok && !layout.isEmpty())
    {
        std::string desiredLayout = layout.toStdString();
        reset(true /*keepData*/);
        setLayout(QString(desiredLayout.c_str()));
        updateStatus(std::string("Changed layers graph view layout to '") + desiredLayout + "'!", DEFAULT_TIMEOUT);
    }
    else
    {
        updateStatus(std::string("Failed to change layers graph view layout: aborted by user!"), DEFAULT_TIMEOUT);
    }
}

void LayerWidget::reset(bool keepData)
{
    clear();

    if(!keepData)
    {
        mpGraph = BaseGraph::Ptr( new gl::DirectedGraph() );
    }

    if(mpGVGraph)
    {
        delete mpGVGraph;
    }
    mpGVGraph = new io::GVGraph(mpGraph, "GVLayerWidget");
}

void LayerWidget::clear()
{
    // TO-DO: add filtering clearing
    if(mpGVGraph)
    {
        mpGVGraph->clearEdges();
        mpGVGraph->clearNodes();
    }

    mNodeItemMap.clear();
    mEdgeItemMap.clear();
    scene()->clear();
}
void LayerWidget::refresh()
{
    updateStatus(std::string("refreshing layers graph view..."));
    reset(true /*keepData*/);
    updateFromGraph();
    update();
    updateStatus(std::string("Refreshed layers graph view!"), DEFAULT_TIMEOUT);
}

void LayerWidget::enableVertex(graph_analysis::Vertex::Ptr vertex)
{
    mpSubGraph->enable(vertex);
    LOG_DEBUG_S << "Enabled a vertex of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void LayerWidget::enableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->enable(edge);
    LOG_DEBUG_S << "Enabled an edge of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void LayerWidget::disableVertex(graph_analysis::Vertex::Ptr vertex)
{
    mpSubGraph->disable(vertex);
    LOG_DEBUG_S << "Disabled vertex '" << vertex->getLabel() << "' of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void LayerWidget::disableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->disable(edge);
    LOG_DEBUG_S << "Disabled edge '" << edge->getLabel() << "' of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void LayerWidget::updateFromGraph()
{
    if(mpGraph->empty())
    {
        LOG_WARN_S << "graph_analysis::gui::LayerWidget::updateFromGraph was called while mpGraph was still empty";
        return;
    }
    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // Check on active filter
        if(mFiltered && !mpSubGraph->enabled(vertex))
        {
            LOG_DEBUG_S << "Filtered out a vertex of filtering value: " << mpSubGraph->enabled(vertex);
            continue;
        }

        if(mNodeItemMap.count(vertex))
        {
            continue;
        }

        if(toggledOut(vertex))
        {
            continue;
        }

        // Registering new node items
        NodeItem* nodeItem = NodeTypeManager::getInstance()->createItem(this, vertex, LAYER_NODE_TYPE);
        mNodeItemMap[vertex] = nodeItem;
        scene()->addItem(nodeItem);
        mpGVGraph->addNode(vertex);
    }
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        // Check on active filter
        if(mFiltered && !mpSubGraph->enabled(edge))
        {
            LOG_DEBUG_S << "Filtered out an edge of filtering value: " << mpSubGraph->enabled(edge);
            continue;
        }

        if(mEdgeItemMap.count(edge))
        {
            continue;
        }

        // Registering new edge items
        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        if(
            toggledOut(source)
                ||
            toggledOut(target)
        )
        {
            continue;
        }

        NodeItem* sourceNodeItem = mNodeItemMap[ source ];
        NodeItem* targetNodeItem = mNodeItemMap[ target ];

        if(!sourceNodeItem || !targetNodeItem)
        {
            continue;
        }

        EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, targetNodeItem, edge, LAYER_EDGE_TYPE);
        mEdgeItemMap[edge] = edgeItem;

        scene()->addItem(edgeItem);
        mpGVGraph->addEdge(edge);
    }

    if(mLayout.toLower() != "force")
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        LOG_INFO_S << "GV started layouting the graph. This can take a while ...";
        base::Time start = base::Time::now();
        mpGVGraph->applyLayout(mLayout.toStdString());
        base::Time delay = base::Time::now() - start;
        QApplication::restoreOverrideCursor();
        LOG_INFO_S << "GV layouted the graph after " << delay.toSeconds();

        {
            using namespace graph_analysis::io;
            std::vector<GVNode> nodes = mpGVGraph->nodes();
            std::vector<GVNode>::const_iterator cit = nodes.begin();
            for(; cit != nodes.end(); ++cit)
            {
                GVNode gvNode = *cit;
                NodeItem* nodeItem = mNodeItemMap[gvNode.getVertex()];

                if(!nodeItem)
                {
                    LOG_WARN_S << "NodeItem: mapped from " <<  gvNode.getVertex()->toString() << "is null";
                }

                nodeItem->setPos(mScaleFactor * gvNode.x(), mScaleFactor * gvNode.y());
            }
        }

//        {
//            using namespace graph_analysis::io;
//            std::vector<GVEdge> edges = mpGVGraph->edges();
//            std::vector<GVEdge>::const_iterator cit = edges.begin();
//            for(; cit != edges.end(); ++cit)
//            {
//                GVEdge gvEdge = *cit;
//                EdgeItem* edgeItem = mEdgeItemMap[ gvEdge.getEdge() ];
//                edgeItem->setPainterPath( edge.path );
//            }
//        }
    }
}

void LayerWidget::addVertex(Vertex::Ptr vertex)
{
    mpGraph->addVertex(vertex);
}

void LayerWidget::addEdge(Edge::Ptr edge)
{
    mpGraph->addEdge(edge);
}

void LayerWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to enable Panning
        setDragMode(ScrollHandDrag);
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mousePressEvent(&fake);
    }
    else QGraphicsView::mousePressEvent(event);
}

void LayerWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to end Panning
        setDragMode(NoDrag);
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mouseReleaseEvent(&fake);
    }
    else QGraphicsView::mouseReleaseEvent(event);
}

void LayerWidget::itemMoved()
{
    if (!mTimerId)
    {
        mTimerId = startTimer(1000 / 25);
    }
}

void LayerWidget::keyPressEvent(QKeyEvent *event)
{
    // check for a combination of user clicks
    Qt::KeyboardModifiers modifiers = event->modifiers();

    if(modifiers & Qt::ControlModifier)
    {
        switch (event->key())
        {
            case Qt::Key_Q: // CTRL+Q and CTRL+W terminate the application
            case Qt::Key_W:
                exit(0);
            break;

            case Qt::Key_Plus: // CTRL+ zooms in
                zoomIn();
            break;

            case Qt::Key_Minus: // CTRL+- zooms out
                zoomOut();
            break;

            case Qt::Key_Left: // CTRL+LeftArrow rotates the view counterclockwise
                rotate(qreal(-1.13));
            break;

            case Qt::Key_Right: // CTRL+RightArrow rotates the view clockwise
                rotate(qreal( 1.13));
            break;

            case Qt::Key_R: // CTRL+R deletes the graph (it first prompts again the user)
                mpViewWidget->resetGraph();
            break;

            case Qt::Key_S: // CTRL+S saves the graph to file
                mpViewWidget->exportGraph();
            break;

            case Qt::Key_I: // CTRL+O or CTRL+I opens a graph from file
            case Qt::Key_O:
                mpViewWidget->importGraph();
            break;

            case Qt::Key_P: // CTRL+P reloads the property dialog (if it is currently not running)
                if(!mpViewWidget->dialogIsRunning())
                {
                    mpViewWidget->reloadPropertyDialog();
                }
            break;
        }
    }
    else
    {
        switch (event->key())
        {
        //case Qt::Key_Up:
        //    break;
        //case Qt::Key_Down:
        //    break;
        //case Qt::Key_Left:
        //    break;
        //case Qt::Key_Right:
        //    break;
        case Qt::Key_Plus: // '+' zooms-in
            zoomIn();
        break;
        case Qt::Key_Minus: // '-' zooms-out
            zoomOut();
        break;
        case Qt::Key_Space: // Space, newline and 'R'/'r' refresh the view
        case Qt::Key_Enter:
        case Qt::Key_R:
                refresh();
        break;

        case Qt::Key_S: // 'S'/'s' shuffle the nodes
            shuffle();
        break;
        //default:
        }
    }

    QGraphicsView::keyPressEvent(event);
}

void LayerWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if(mLayout.toLower() == "force")
    {
        QList<NodeItem* > nodes;
        foreach (QGraphicsItem *item, scene()->items())
        {
            if (NodeItem* node = qgraphicsitem_cast<NodeItem* >(item))
            {
                nodes << node;
            }
        }

        foreach (NodeItem* node, nodes)
        {
            node->calculateForces();
        }

        bool itemsMoved = false;
        foreach (NodeItem* node, nodes)
        {
            if (node->advance())
                itemsMoved = true;
        }

        if (!itemsMoved)
        {
            killTimer(mTimerId);
            mTimerId = 0;
        }
    }
}

#ifndef QT_NO_WHEELEVENT
void LayerWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, - event->delta() / 240.0));
}
#endif

void LayerWidget::drawBackground(QPainter *painter, const QRectF& rect)
{
    Q_UNUSED(rect);

    //// Shadow
//    QRectF sceneRect = this->sceneRect();
    //QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    //QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    //if (rightShadow.intersects(rect) || rightShadow.contains(rect))
    //    painter->fillRect(rightShadow, Qt::darkGray);
    //if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
    //    painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    //QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    //gradient.setColorAt(0, Qt::white);
    //gradient.setColorAt(1, Qt::lightGray);
    //painter->fillRect(rect.intersected(sceneRect), gradient);
    //painter->setBrush(Qt::NoBrush);
    //painter->drawRect(sceneRect);

    //// Text
    //QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
    //                sceneRect.width() - 4, sceneRect.height() - 4);
    //QString message(tr("Click and drag the nodes around, and zoom with the mouse "
    //                   "wheel or the '+' and '-' keys"));

    //QFont font = painter->font();
    //font.setBold(true);
    //font.setPointSize(14);
    //painter->setFont(font);
    //painter->setPen(Qt::lightGray);
    //painter->drawText(textRect.translated(2, 2), message);
    //painter->setPen(Qt::black);
    //painter->drawText(textRect, message);
}

void LayerWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
    {
        return;
    }
    scale(scaleFactor, scaleFactor);
    std::string status_msg = scaleFactor > 1. ? "Zoomed-in" : "Zoomed-out";
    updateStatus(status_msg, DEFAULT_TIMEOUT);
}

void LayerWidget::setNodeFilters(std::vector< Filter<Vertex::Ptr>::Ptr > filters)
{
    mpVertexFilter->clear();

    BOOST_FOREACH(Filter<Vertex::Ptr>::Ptr filter, filters)
    {
        mpVertexFilter->add(filter);
    }
    mGraphView.setVertexFilter(mpVertexFilter);
    if(!mFiltered)
    {
        mpSubGraph = mGraphView.apply(mpGraph);
        mFiltered = true;
    }
}

void LayerWidget::setEdgeFilters(std::vector< Filter<Edge::Ptr>::Ptr > filters)
{
    mpEdgeFilter->clear();

    BOOST_FOREACH(Filter<Edge::Ptr>::Ptr filter, filters)
    {
        mpEdgeFilter->add(filter);
    }
    mGraphView.setEdgeFilter(mpEdgeFilter);
    if(!mFiltered)
    {
        mpSubGraph = mGraphView.apply(mpGraph);
        mFiltered = true;
    }
}

void LayerWidget::shuffle()
{
    updateStatus(std::string("shuflling all the nodes in the layers graph view..."));
    foreach (QGraphicsItem *item, scene()->items())
    {
        if (qgraphicsitem_cast<NodeItem* >(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
    updateStatus(std::string("Shuflled all nodes in the layers graph view!"), DEFAULT_TIMEOUT);
}

void LayerWidget::zoomIn()
{
    scaleView(qreal(1.13));
}

void LayerWidget::zoomOut()
{
    scaleView(1 / qreal(1.13));
}

void LayerWidget::setLayout(QString layoutName)
{
    mLayout = layoutName;
    updateFromGraph();
}

void LayerWidget::togglePortLayer(bool toggle)
{
    mPortLayerToggle = toggle;
    refresh();
}

void LayerWidget::toggleClusterLayer(bool toggle)
{
    mClusterLayerToggle = toggle;
    refresh();
}

} // end namespace gui
} // end namespace graph_analysis
