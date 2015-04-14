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
#include <QKeyEvent>
#include <QTime>
#include <QApplication>
#include <boost/regex.hpp>
#include <base/Logging.hpp>
#include <graph_analysis/GraphView.hpp>

#include "GVGraph.hpp"
#include <graph_analysis/Filter.hpp>

#include <graph_analysis/lemon/Graph.hpp>
#include <boost/foreach.hpp>
#include <base/Time.hpp>

using namespace graph_analysis;
namespace gl = graph_analysis::lemon;

namespace graph_analysis {
namespace gui {

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
    , mpGraph(0)
    , mpGVGraph(0)
    , mTimerId(0)
    , mLayout("dot")
    , mpVertexFilter(new graph_analysis::Filter< graph_analysis::Vertex::Ptr>())
    , mpEdgeFilter(new graph_analysis::EdgeContextFilter())
{
    // Add seed for force layout
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Graphview"));

    reset();

}

void GraphWidget::toFile(const std::string &filename)
{
    mpGVGraph->renderToFile(filename, mLayout.toStdString());
}

void GraphWidget::reset(bool keepData)
{
    clear();

    delete mpGVGraph;
    mpGVGraph = new GVGraph("GVGraphWidget");

    if(!keepData)
    {
        delete mpGraph;
        mpGraph = new gl::DirectedGraph();
    }
}

void GraphWidget::clear()
{
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
}
void GraphWidget::refresh()
{
    reset(true /*keepData*/);
    updateFromGraph();
    update();
}

void GraphWidget::updateFromGraph()
{
    // Setting up filtering
    GraphView< gl::DirectedGraph > graphView;
    graphView.setVertexFilter(mpVertexFilter);
    graphView.setEdgeFilter(mpEdgeFilter);
    // End of setting up filters

    gl::DirectedSubGraph subGraph = graphView.apply(*dynamic_cast<gl::DirectedGraph*>(mpGraph));

    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // Check on active filter
        if(!subGraph.enabled(vertex))
        {
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
        if(!subGraph.enabled(edge))
        {
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

        EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(sourceNodeItem, targetNodeItem, edge);
        mEdgeItemMap[edge] = edgeItem;

        scene()->addItem(edgeItem);
        mpGVGraph->addEdge(QString( sourceNodeItem->getId().c_str()), QString( targetNodeItem->getId().c_str()));
        mGVEdgeItemMap[edgeItem->getId()] = edgeItem;
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

        foreach(GVNode node, mpGVGraph->nodes())
        {
            NodeItem* nodeItem = mGVNodeItemMap[ node.name.toStdString() ];

            if(!nodeItem)
            {
                LOG_WARN_S << "NodeItem: " << node.name.toStdString() << "is null";
            }

            //QPointF p = mapFromScene(nodeItem->pos());
            QPointF p = nodeItem->pos();
            QPointF scenePos = nodeItem->scenePos();
            QPointF position = node.centerPos;
            nodeItem->setPos(position.x(), position.y());
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
}

void GraphWidget::addEdge(Edge::Ptr edge)
{
    mpGraph->addEdge(edge);
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
    switch (event->key()) {
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
        foreach (NodeItem* node, nodes) {
            if (node->advance())
                itemsMoved = true;
        }

        if (!itemsMoved) {
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
    QRectF sceneRect = this->sceneRect();
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
}

void GraphWidget::setEdgeFilters(std::vector< Filter<Edge::Ptr>::Ptr > filters)
{
    mpEdgeFilter->clear();

    BOOST_FOREACH(Filter<Edge::Ptr>::Ptr filter, filters)
    {
        mpEdgeFilter->add(filter);
    }
}

void GraphWidget::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items()) {
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
    updateFromGraph();
}

} // end namespace gui
} // end namespace graph_analysis
