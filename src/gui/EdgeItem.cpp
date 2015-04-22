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

#include <math.h>
#include <QPainter>
#include <base/Logging.hpp>

const double graph_analysis::gui::EdgeItem::Pi = 3.14159265358979323846264338327950288419717;
double graph_analysis::gui::EdgeItem::TwoPi = 2.0 * Pi;

namespace graph_analysis {
namespace gui {

EdgeItem::EdgeItem(NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge)
    : mpSourceNodeItem(sourceNode)
    , mpTargetNodeItem(targetNode)
    , mpEdge(edge)
    , mArrowSize(10)
{
    setAcceptedMouseButtons(0);
    adjust();
}

NodeItem* EdgeItem::sourceNodeItem() const
{
    return mpSourceNodeItem;
}

NodeItem* EdgeItem::targetNodeItem() const
{
    return mpTargetNodeItem;
}

void EdgeItem::adjust()
{
    if (!mpSourceNodeItem || !mpTargetNodeItem)
        return;

    QLineF line(mapFromItem(mpSourceNodeItem, 0, 0), mapFromItem(mpTargetNodeItem, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.))
    {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        mSourcePoint = line.p1() + edgeOffset;
        mTargetPoint = line.p2() - edgeOffset;
    } else {
        mSourcePoint = mTargetPoint = line.p1();
    }
}

QRectF EdgeItem::boundingRect() const
{
    if (!mpSourceNodeItem || !mpTargetNodeItem)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + mArrowSize) / 2.0;

    return QRectF(mSourcePoint, QSizeF(mTargetPoint.x() - mSourcePoint.x(),
                                      mTargetPoint.y() - mSourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void EdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!mpSourceNodeItem || !mpTargetNodeItem)
        return;

    QLineF line(mSourcePoint, mTargetPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF sourceArrowP1 = mSourcePoint + QPointF(sin(angle + Pi / 3) * mArrowSize,
                                                  cos(angle + Pi / 3) * mArrowSize);
    QPointF sourceArrowP2 = mSourcePoint + QPointF(sin(angle + Pi - Pi / 3) * mArrowSize,
                                                  cos(angle + Pi - Pi / 3) * mArrowSize);
    QPointF destArrowP1 = mTargetPoint + QPointF(sin(angle - Pi / 3) * mArrowSize,
                                              cos(angle - Pi / 3) * mArrowSize);
    QPointF destArrowP2 = mTargetPoint + QPointF(sin(angle - Pi + Pi / 3) * mArrowSize,
                                              cos(angle - Pi + Pi / 3) * mArrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}

QPainterPath EdgeItem::shape() const 
{
    return mPainterPath;
    return ::QGraphicsItem::shape();
}

std::string EdgeItem::getId() const
{
    char buffer[512];
    snprintf(buffer, 512, "%s->%s",mpSourceNodeItem->getId().c_str(), mpTargetNodeItem->getId().c_str());
    return std::string(buffer);
}

} // end namespace gui
} // end namespace graph_analysis
