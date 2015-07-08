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

#ifndef GRAPH_ANALYSIS_GUI_EDGEITEM_HPP
#define GRAPH_ANALYSIS_GUI_EDGEITEM_HPP

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QPainter>
#include <math.h>
#include <graph_analysis/Edge.hpp>
#include <graph_analysis/gui/graphitem/edges/EdgeLabel.hpp>

namespace graph_analysis {
namespace gui {

class GraphWidget;
class NodeItem;

class EdgeItem : public QGraphicsItemGroup
{
public:
    EdgeItem() {}
    ~EdgeItem() {}

    EdgeItem(GraphWidget* graphWidget, NodeItem* sourceNode, NodeItem* destNode, graph_analysis::Edge::Ptr edge);

    NodeItem* sourceNodeItem() const;
    NodeItem* targetNodeItem() const;

    virtual EdgeItem* createNewItem(GraphWidget* graphWidget, NodeItem* sourceNode, int sourceNodePortID, NodeItem* targetNode, int targetNodePortID, graph_analysis::Edge::Ptr edge) const { throw std::runtime_error("graph_analysis::gui::EdgeItem::createNewItem is not reimplemented"); }

    virtual void adjust();

    enum { Type = UserType + 2 };
    int type() const { return Type; }

    void setPainterPath(QPainterPath painterPath) { mPainterPath = painterPath; }
    virtual graphitem::edges::EdgeLabel* getLabel() {  throw std::runtime_error("graph_analysis::gui::EdgeItem: edge label is not implemented at this level"); }
    virtual int getSourcePortID() { throw std::runtime_error("graph_analysis::gui::EdgeItem::getSourcePortID: is not reimplemented at this level"); }
    virtual int getTargetPortID() { throw std::runtime_error("graph_analysis::gui::EdgeItem::getTargetPortID: is not reimplemented at this level"); }
    virtual void releaseFocus()   { throw std::runtime_error("graph_analysis::gui::EdgeItem::releaseFocus is not reimplemented");}
    virtual void adjustLabel()    { throw std::runtime_error("graph_analysis::gui::EdgeItem::adjustLabel is not reimplemented"); }

    graph_analysis::Edge::Ptr getEdge() { return mpEdge; }

protected:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    virtual QPainterPath shape() const;

    GraphWidget* mpGraphWidget;
    NodeItem* mpSourceNodeItem;
    NodeItem* mpTargetNodeItem;
    graph_analysis::Edge::Ptr mpEdge;

    QPointF mSourcePoint;
    QPointF mTargetPoint;
    qreal mArrowSize;

    static const double Pi;
    static double TwoPi;

    QPainterPath mPainterPath;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_EDGEITEM_HPP
