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

#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "GraphWidget.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include <base/Logging.hpp>
#include <boost/assign/list_of.hpp>

namespace graph_analysis {
namespace gui {

NodeItem::NodeItem(GraphWidget *graphWidget, graph_analysis::Vertex::Ptr vertex)
    : mpVertex(vertex)
    , mpGraphWidget(graphWidget)
{
    setFlag(ItemIsMovable, false);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setToolTip(QString(vertex->getClassName().c_str()));
}

void NodeItem::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this)
    {
        mNewPos = pos();
        return;
    }

    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items())
    {
        NodeItem* node = qgraphicsitem_cast<NodeItem* >(item);
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0)
        {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    // Now subtract all forces pulling items together
    GraphWidget::EdgeItemMap::iterator it = mpGraphWidget->edgeItemMap().begin();

    double weight = (mpGraphWidget->edgeItemMap().size() + 1) * 10;
    for(; it != mpGraphWidget->edgeItemMap().end(); ++it)
    {
        EdgeItem* edge = it->second;
        QPointF vec;
        if (edge->sourceNodeItem() == this)
            vec = mapToItem(edge->targetNodeItem(), 0, 0);
        else
            vec = mapToItem(edge->sourceNodeItem(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    mNewPos = pos() + QPointF(xvel, yvel);
    mNewPos.setX(qMin(qMax(mNewPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    mNewPos.setY(qMin(qMax(mNewPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}

bool NodeItem::advance()
{
    if (mNewPos == pos())
        return false;

    setPos(mNewPos);
    return true;
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch (change)
    {
        case ItemPositionHasChanged:
        {
            GraphWidget::EdgeItemMap::iterator it = mpGraphWidget->edgeItemMap().begin();
            for(; it != mpGraphWidget->edgeItemMap().end(); ++it)
            {
                EdgeItem* edge = it->second;
                edge->adjust();
            }
            mpGraphWidget->itemMoved();
            break;
        }
        default:
            break;
    };

    return QGraphicsItem::itemChange(change, value);
}

QPointF NodeItem::getCenterPosition() const
{
    QRectF rect = boundingRect();
    QPointF position = pos();

    qreal width = rect.width()/2.0;
    qreal height = rect.height()/2.0;
    return QPointF( position.x() + width, position.y() + height);
}

std::string NodeItem::getId() const
{
    char buffer[512];
    snprintf(buffer,512, "%p-%s",this, mpVertex->toString().c_str());
    return std::string(buffer);
}

} // end namespace gui
} // end namespace graph_analysis
