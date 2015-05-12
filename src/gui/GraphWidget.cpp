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

#include "GraphWidget.hpp"
#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "NodeTypeManager.hpp"
#include "EdgeTypeManager.hpp"

#include <math.h>
#include <sstream>
#include <QDir>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSignalMapper>
#include <boost/regex.hpp>
#include <base/Logging.hpp>

#include "GVGraph.hpp"
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>
#include <graph_analysis/gui/graphitem/edges/EdgeLabel.hpp>

#include <boost/foreach.hpp>
#include <base/Time.hpp>
#define DEFAULT_SCALING_FACTOR 1.4
#define UNVALIDATE " "

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
    , mpGraph()
    , mpGVGraph(0)
    , mFiltered(false)
    , mTimerId(0)
    , mScaleFactor(DEFAULT_SCALING_FACTOR)
    , mLayout("dot")
    , mpVertexFilter(new Filter< graph_analysis::Vertex::Ptr>())
    , mpEdgeFilter(new filters::EdgeContextFilter())
    , mVertexSelected(false)
    , mEdgeSelected(false)
    , mEdgeStartVertex(false)
    , mEdgeEndVertex(false)
    , mGVGraphDirty(true)
{
    // Add seed for force layout
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    setCacheMode(CacheBackground);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setViewportUpdateMode(BoundingRectViewportUpdate);
//    setRenderHint(QPainter::Antialiasing);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
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

    reset();
}

void GraphWidget::showContextMenu(const QPoint &pos)
{
    QPoint position = mapTo(this, pos);
    QMenu contextMenu(tr("Context menu"), this);
//    {
        QAction actionChangeEdgeLabel("Change Selected Edge Label", this);
        connect(&actionChangeEdgeLabel, SIGNAL(triggered()), this, SLOT(changeSelectedEdgeLabel()));
        if(mEdgeSelected)
        {
            contextMenu.addAction(&actionChangeEdgeLabel);
        }
//    }
//    {
        QAction actionRemoveEdge("Remove Selected Edge", this);
        connect(&actionRemoveEdge, SIGNAL(triggered()), this, SLOT(removeSelectedEdge()));
        if(mEdgeSelected)
        {
            contextMenu.addAction(&actionRemoveEdge);
        }
//    }
//    {
        QAction actionChangeLabel("Change Selected Node Label", this);
        connect(&actionChangeLabel, SIGNAL(triggered()), this, SLOT(changeSelectedVertexLabel()));
        if(mVertexSelected)
        {
            contextMenu.addAction(&actionChangeLabel);
        }
//    }
//    {
        QAction actionRemoveNode("Remove Selected Node", this);
        connect(&actionRemoveNode, SIGNAL(triggered()), this, SLOT(removeSelectedVertex()));
        if(mVertexSelected)
        {
            contextMenu.addAction(&actionRemoveNode);
        }
//    }
//    {
        QAction actionStartNewEdgeHere("Start New Edge Here", this);
        connect(&actionStartNewEdgeHere, SIGNAL(triggered()), this, SLOT(startNewEdgeHere()));
        if(mVertexSelected)
        {
            contextMenu.addAction(&actionStartNewEdgeHere);
        }
//    }
//    {
        QAction actionEndNewEdgeHere("End New Edge Here", this);
        connect(&actionEndNewEdgeHere, SIGNAL(triggered()), this, SLOT(endNewEdgeHere()));
        if(mVertexSelected)
        {
            contextMenu.addAction(&actionEndNewEdgeHere);
        }
//    }
//    {
        QSignalMapper* signalMapper = new QSignalMapper(this);
        QAction actionAddNode("Add Node", this);
        connect(&actionAddNode, SIGNAL(triggered()), signalMapper, SLOT(map()));
        contextMenu.addAction(&actionAddNode);
        signalMapper->setMapping(&actionAddNode, (QObject*)&position);
        connect(signalMapper, SIGNAL(mapped(QObject*)), this, SLOT(addNodeAdhoc(QObject*)));
//    }
//    {
        QAction actionRefresh("Refresh", this);
        connect(&actionRefresh, SIGNAL(triggered()), this, SLOT(refresh()));
        contextMenu.addAction(&actionRefresh);
//    }
//    {
        QAction actionShuffle("Shuffle", this);
        connect(&actionShuffle, SIGNAL(triggered()), this, SLOT(shuffle()));
        contextMenu.addAction(&actionShuffle);
//    }
//    {
        QAction actionLayout("Change Layout", this);
        connect(&actionLayout, SIGNAL(triggered()), this, SLOT(changeLayout()));
        contextMenu.addAction(&actionLayout);
//    }
//    {
        QAction actionExportGraph("Export Graph", this);
        connect(&actionExportGraph, SIGNAL(triggered()), this, SLOT(exportGraph()));
        contextMenu.addAction(&actionExportGraph);
//    }
    contextMenu.exec(mapToGlobal(pos));
}

void GraphWidget::exportGraph()
{
    bool ok;
    QString label = QInputDialog::getText(this, tr("Name Ouput File"),
                                         tr("export file name:"), QLineEdit::Normal,
                                         "graph.dot", &ok);
    if (ok && !label.isEmpty())
    {
        // TODO: listen to stderr for graphviz errors and alert the user with a message box
        toFileNoLayout(label.toStdString());
    }
}

void GraphWidget::addNodeAdhoc(QObject *pos)
{
    graph_analysis::Vertex::Ptr vertex(new graph_analysis::Vertex());
    mpGraph->addVertex(vertex);
    mGVGraphDirty = true;
    enableVertex(vertex);
    // Registering the new node item
    NodeItem* nodeItem = NodeTypeManager::getInstance()->createItem(this, vertex);
    QPoint *position = (QPoint *)pos;
    nodeItem->setPos((double) position->x(), (double) position->y());
    mNodeItemMap[vertex] = nodeItem;

    scene()->addItem(nodeItem);
//    update();
}

void GraphWidget::changeSelectedVertexLabel()
{
    bool ok;
    QString label = QInputDialog::getText(this, tr("Input Node Label"),
                                         tr("New Label:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !label.isEmpty())
    {
        mpSelectedVertex->setLabel(label.toStdString());
        NodeItem* nodeItem = mNodeItemMap[mpSelectedVertex];
        nodeItem->updateLabel();
    }
}

void GraphWidget::changeSelectedEdgeLabel()
{
    bool ok;
    QString label = QInputDialog::getText(this, tr("Input Edge Label"),
                                         tr("New Label:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !label.isEmpty())
    {
        EdgeItem* edge = mEdgeItemMap[mpSelectedEdge];
        graphitem::edges::EdgeLabel* edgeLabel = edge->getLabel();
        edgeLabel->setPlainText(QString(label.toStdString().c_str()));
        mpSelectedEdge->setLabel(label.toStdString());
    }
}

void GraphWidget::removeSelectedEdge()
{
    //    bool bidirectional = false;
    const Vertex::Ptr source = mpSelectedEdge->getSourceVertex();
    const Vertex::Ptr target = mpSelectedEdge->getTargetVertex();
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(target);
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        const Vertex::Ptr target = edge->getTargetVertex();
        if(target == source)
        {
//            bidirectional = true;
            EdgeItem* edgeItem = mEdgeItemMap[edge];
            graphitem::edges::EdgeLabel* edgeLabel = edgeItem->getLabel();
            edgeLabel->revertText();
            edge->revertLabel();
            break;
        }
    }

    scene()->removeItem(mEdgeItemMap[mpSelectedEdge]);
    mpGraph->removeEdge(mpSelectedEdge);
    mGVGraphDirty = true;
}

void GraphWidget::removeSelectedVertex()
{
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(mpSelectedVertex);
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        scene()->removeItem(mEdgeItemMap[edge]);
        mpGraph->removeEdge(edge);
    }
    scene()->removeItem(mNodeItemMap[mpSelectedVertex]);
    mpGraph->removeVertex(mpSelectedVertex);
    mGVGraphDirty = true;
}

void GraphWidget::changeLayout()
{
    bool ok;
    QStringList options;
    options << tr("circo") << tr("dot") << tr("fdp") << tr("neato") << tr("osage") << tr("sfdp") << tr("twopi"); // [circo, dot, fdp, neato, osage, sfdp, twopi]
    QString layout = QInputDialog::getItem(this, tr("Input New Layout"),
                                         tr("select a layout:"), options,
                                         0, false, &ok);
    if (ok && !layout.isEmpty())
    {
        std::string desiredLayout = layout.toStdString();
        std::set<std::string> layouts = GVGraph::getSupportedLayouts();
        if(layouts.end() == layouts.find(desiredLayout))
        {
            /*
            Error: Layout type: "two" not recognized. Use one of: circo dot fdp neato nop nop1 nop2 osage patchwork sfdp twopi
             */
            std::stringstream errorMessage;
            errorMessage << "Error: Layout type: \"" << desiredLayout << "\" not recognized. Use one of: ";
            std::set<std::string>::iterator it = layouts.begin();
            for(; layouts.end() != it; ++it)
            {
                errorMessage << *it << ' ';
            }
            QMessageBox::critical(this, QString("Layouting Error"), QString(errorMessage.str().c_str()));
        }
        else
        {
            reset(true /*keepData*/);
            setLayout(QString(desiredLayout.c_str()));
        }
    }
}

void GraphWidget::startNewEdgeHere()
{
    mpStartVertex = mpSelectedVertex;
    mEdgeStartVertex = true;
    if(mEdgeStartVertex && mEdgeEndVertex)
    {
        bool ok;
        QString label = QInputDialog::getText(this, tr("Input New Edge Label"),
                                             tr("New Edge Label:"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
        if (ok && !label.isEmpty())
        {
            spawnEdge(label.toStdString());
        }
        mEdgeStartVertex    = false;
        mEdgeEndVertex      = false;
    }
}

void GraphWidget::spawnEdge(const std::string &label)
{
    bool bidirectional = false;
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(mpEndVertex);
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        const Vertex::Ptr target = edge->getTargetVertex();
        if(target == mpStartVertex)
        {
            bidirectional = true;
            EdgeItem* edgeItem = mEdgeItemMap[edge];
            graphitem::edges::EdgeLabel* edgeLabel = edgeItem->getLabel();
            edgeLabel->setText(QString(UNVALIDATE));
            edge->setLabel(UNVALIDATE);
            edge->unSetActive();
            break;
        }
    }

    Edge::Ptr edge(new Edge());
    edge->setSourceVertex(mpStartVertex);
    edge->setTargetVertex(mpEndVertex);
    if(bidirectional)
    {
        edge->setActive();
    }
    mpGraph->addEdge(edge);
    mGVGraphDirty = true;
    enableEdge(edge);
    // Registering new node edge items
    Vertex::Ptr source = edge->getSourceVertex();
    Vertex::Ptr target = edge->getTargetVertex();

    NodeItem* sourceNodeItem = mNodeItemMap[ source ];
    NodeItem* targetNodeItem = mNodeItemMap[ target ];

    if(sourceNodeItem && targetNodeItem)
    {
        EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, targetNodeItem, edge);
        mEdgeItemMap[edge] = edgeItem;
        graphitem::edges::EdgeLabel* edgeLabel = edgeItem->getLabel();
        edgeLabel->setPlainText(QString(label.c_str()));
        edge->setLabel(label);
        scene()->addItem(edgeItem);
        edgeItem->adjust();
    }
}

void GraphWidget::endNewEdgeHere()
{
    mpEndVertex = mpSelectedVertex;
    mEdgeEndVertex = true;
    if(mEdgeStartVertex && mEdgeEndVertex)
    {
        bool ok;
        QString label = QInputDialog::getText(this, tr("Input New Edge Label"),
                                             tr("New Edge Label:"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
        if (ok && !label.isEmpty())
        {
            spawnEdge(label.toStdString());
        }
        mEdgeStartVertex    = false;
        mEdgeEndVertex      = false;
    }
}

void GraphWidget::toFile(const std::string &filename)
{
    if(mGVGraphDirty)
    {
        updateGVGraph();
    }
    mpGVGraph->renderToFile(filename, mLayout.toStdString());
}

void GraphWidget::toFileNoLayout(const std::string &filename)
{
    if(mGVGraphDirty)
    {
        updateGVGraph();
    }
    mpGVGraph->renderToFileNoLayout(filename);
}

void GraphWidget::reset(bool keepData)
{
    clear();
    if(!keepData)
    {
        mpGraph = BaseGraph::Ptr( new gl::DirectedGraph() );
    }
}

void GraphWidget::clear()
{
    // TO-DO: add filtering clearing
    if(mpGVGraph)
    {
        mpGVGraph->clearEdges();
        mpGVGraph->clearNodes();
    }

    mGVNodeItemMap.clear();
    mGVEdgeItemMap.clear();
    mNodeItemMap.clear();
    mEdgeItemMap.clear();
    scene()->clear();
    mGVGraphDirty = true;
}
void GraphWidget::refresh()
{
    reset(true /*keepData*/);
    updateFromGraph();
    update();
}

void GraphWidget::enableVertex(graph_analysis::Vertex::Ptr vertex)
{
    LOG_DEBUG_S << "Enabling a vertex of filtering value: " << mpSubGraph->enabled(vertex);
    mpSubGraph->enable(vertex);
    LOG_DEBUG_S << "Enabled the vertex; NOW of filtering value: " << mpSubGraph->enabled(vertex);
}
void GraphWidget::enableEdge(graph_analysis::Edge::Ptr edge)
{
    LOG_DEBUG_S << "Enabling an edge of filtering value: " << mpSubGraph->enabled(edge);
    mpSubGraph->enable(edge);
    LOG_DEBUG_S << "Enabled the edge; NOW of filtering value: " << mpSubGraph->enabled(edge);
}

void GraphWidget::updateGVGraph()
{
    if(mpGVGraph)delete mpGVGraph;
    mpGVGraph = new GVGraph("GVGraphWidget");

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

        if( mNodeItemMap.count(vertex))
        {
            continue;
        }

        // Registering new node items
        NodeItem* nodeItem = mNodeItemMap[vertex];
        mpGVGraph->addNode(QString( nodeItem->getId().c_str()));
        mGVNodeItemMap[nodeItem->getId()] = nodeItem;
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

        if( mEdgeItemMap.count(edge))
        {
            continue;
        }

        // Registering new node edge items
        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        NodeItem* sourceNodeItem = mNodeItemMap[ source ];
        NodeItem* targetNodeItem = mNodeItemMap[ target ];

        if(!sourceNodeItem || !targetNodeItem)
        {
            continue;
        }

        EdgeItem* edgeItem = mEdgeItemMap[edge];
        mpGVGraph->addEdge(QString( sourceNodeItem->getId().c_str()), QString( targetNodeItem->getId().c_str()));
        mGVEdgeItemMap[edgeItem->getId()] = edgeItem;
    }
    mGVGraphDirty = false;
}

void GraphWidget::standAloneLayouting()
{
    if(mGVGraphDirty)
    {
        updateFromGraph();
    }
    else
    {
        if(mLayout.toLower() != "force")
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);

            LOG_INFO_S << "GV started layouting the graph. This can take a while ...";
            base::Time start = base::Time::now();
            mpGVGraph->applyLayout(mLayout.toStdString());
            base::Time delay = base::Time::now() - start;
            QApplication::restoreOverrideCursor();
            LOG_INFO_S << "GV layouted the graph after " << delay.toSeconds();

            foreach(GVNode node, mpGVGraph->nodes())
            {
                NodeItem* nodeItem = mGVNodeItemMap[ node.name.toStdString() ];

                if(!nodeItem)
                {
                    LOG_WARN_S << "NodeItem: " << node.name.toStdString() << "is null";
                }

    //            QPointF p = mapFromScene(nodeItem->pos());
    //            QPointF p = nodeItem->pos();
    //            QPointF scenePos = nodeItem->scenePos();
                QPointF position = node.centerPos;
                nodeItem->setPos(mScaleFactor * position.x(), mScaleFactor * position.y());
            }

            foreach(GVEdge edge, mpGVGraph->edges())
            {
                EdgeItem* edgeItem = mGVEdgeItemMap[ edge.getId().toStdString() ];
                edgeItem->setPainterPath( edge.path );
            }
        }
    }
}

void GraphWidget::updateFromGraph()
{
    if(mpGVGraph)delete mpGVGraph;
    mpGVGraph = new GVGraph("GVGraphWidget");

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

        if( mNodeItemMap.count(vertex))
        {
            continue;
        }

        // Registering new node items
        NodeItem* nodeItem = NodeTypeManager::getInstance()->createItem(this, vertex);
        mNodeItemMap[vertex] = nodeItem;

        scene()->addItem(nodeItem);
        mpGVGraph->addNode(QString( nodeItem->getId().c_str()));
        mGVNodeItemMap[nodeItem->getId()] = nodeItem;
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

        if( mEdgeItemMap.count(edge))
        {
            continue;
        }

        // Registering new node edge items
        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        NodeItem* sourceNodeItem = mNodeItemMap[ source ];
        NodeItem* targetNodeItem = mNodeItemMap[ target ];

        if(!sourceNodeItem || !targetNodeItem)
        {
            continue;
        }

        EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, targetNodeItem, edge);
        mEdgeItemMap[edge] = edgeItem;

        scene()->addItem(edgeItem);
        mpGVGraph->addEdge(QString( sourceNodeItem->getId().c_str()), QString( targetNodeItem->getId().c_str()));
        mGVEdgeItemMap[edgeItem->getId()] = edgeItem;
    }

    mGVGraphDirty = false;

    if(mLayout.toLower() != "force")
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        LOG_INFO_S << "GV started layouting the graph. This can take a while ...";
        base::Time start = base::Time::now();
        mpGVGraph->applyLayout(mLayout.toStdString());
        base::Time delay = base::Time::now() - start;
        QApplication::restoreOverrideCursor();
        LOG_INFO_S << "GV layouted the graph after " << delay.toSeconds();

        foreach(GVNode node, mpGVGraph->nodes())
        {
            NodeItem* nodeItem = mGVNodeItemMap[ node.name.toStdString() ];

            if(!nodeItem)
            {
                LOG_WARN_S << "NodeItem: " << node.name.toStdString() << "is null";
            }

//            QPointF p = mapFromScene(nodeItem->pos());
//            QPointF p = nodeItem->pos();
//            QPointF scenePos = nodeItem->scenePos();
            QPointF position = node.centerPos;
            nodeItem->setPos(mScaleFactor * position.x(), mScaleFactor * position.y());
        }

        foreach(GVEdge edge, mpGVGraph->edges())
        {
            EdgeItem* edgeItem = mGVEdgeItemMap[ edge.getId().toStdString() ];
            edgeItem->setPainterPath( edge.path );
        }
    }
}

void GraphWidget::addVertex(Vertex::Ptr vertex)
{
    mpGraph->addVertex(vertex);
    mGVGraphDirty = true;
}

void GraphWidget::addEdge(Edge::Ptr edge)
{
    mpGraph->addEdge(edge);
    mGVGraphDirty = true;
}

void GraphWidget::itemMoved()
{
    if (!mTimerId)
    {
        mTimerId = startTimer(1000 / 25);
    }
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
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
    //case Qt::Key_Plus:
    //    zoomIn();
    //    break;
    //case Qt::Key_Minus:
    ////    zoomOut();
    //    break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
            refresh();
    ////    shuffle();
        break;
    //default:
    }

    QGraphicsView::keyPressEvent(event);
}

void GraphWidget::timerEvent(QTimerEvent *event)
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
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
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

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::setNodeFilters(std::vector< Filter<Vertex::Ptr>::Ptr > filters)
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

void GraphWidget::setEdgeFilters(std::vector< Filter<Edge::Ptr>::Ptr > filters)
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

void GraphWidget::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items())
    {
        if (qgraphicsitem_cast<NodeItem* >(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void GraphWidget::setLayout(QString layoutName)
{
    mLayout = layoutName;
    standAloneLayouting();
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
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

void GraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to enable Panning
        setDragMode(NoDrag);
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mouseReleaseEvent(&fake);
    }
    else QGraphicsView::mouseReleaseEvent(event);
}

} // end namespace gui
} // end namespace graph_analysis
