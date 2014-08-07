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
#include <base/Logging.hpp>

namespace omviz {

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
    , mTimerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);

    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Graphview"));
}

void GraphWidget::updateFromGraph()
{
    graph_analysis::VertexIterator::Ptr nodeIt = mGraph.getVertexIterator();
    while(nodeIt->next())
    {
        graph_analysis::Vertex::Ptr vertex = nodeIt->current();
        if( mNodeItemMap.count(vertex))
        {
            continue;
        }

        // Registering new node items
        NodeItem* nodeItem = NodeTypeManager::getInstance()->createItem(this, vertex);
        mNodeItemMap[vertex] = nodeItem;
        scene()->addItem(nodeItem);
    }

    graph_analysis::EdgeIterator::Ptr edgeIt = mGraph.getEdgeIterator();
    while(edgeIt->next())
    {
        graph_analysis::Edge::Ptr edge = edgeIt->current();
        if( mEdgeItemMap.count(edge))
        {
            continue;
        }

        // Registering new node edge items
        graph_analysis::Vertex::Ptr source = edge->getSourceVertex();
        graph_analysis::Vertex::Ptr target = edge->getTargetVertex();

        NodeItem* sourceNodeItem = mNodeItemMap[ source ]; 
        NodeItem* targetNodeItem = mNodeItemMap[ target ];

        EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(sourceNodeItem, targetNodeItem, edge);
        mEdgeItemMap[edge] = edgeItem;

        scene()->addItem(edgeItem);
    }
}

void GraphWidget::addVertex(graph_analysis::Vertex::Ptr vertex)
{ 
    mGraph.addVertex(vertex);
}

void GraphWidget::addEdge(graph_analysis::Edge::Ptr edge)
{
    mGraph.addEdge(edge);
}

void GraphWidget::itemMoved()
{
    if (!mTimerId)
        mTimerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    //switch (event->key()) {
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
    //case Qt::Key_Space:
    //case Qt::Key_Enter:
    ////    shuffle();
    //    break;
    //default:
    //}

    QGraphicsView::keyPressEvent(event);
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    //QList<NodeItem* > nodes;
    //foreach (QGraphicsItem *item, scene()->items()) {
    //    if (NodeItem* node = qgraphicsitem_cast<NodeItem* >(item))
    //        nodes << node;
    //}

    //foreach (NodeItem* node, nodes)
    //    node->calculateForces();

    //bool itemsMoved = false;
    //foreach (NodeItem* node, nodes) {
    //    if (node->advance())
    //        itemsMoved = true;
    //}

    //if (!itemsMoved) {
    //    killTimer(mTimerId);
    //    mTimerId = 0;
    //}
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

} // end namespace omviz
