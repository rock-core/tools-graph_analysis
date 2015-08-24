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
#include <graph_analysis/gui/layeritem/edges/EdgeLabel.hpp>

namespace graph_analysis {
namespace gui {

class GraphWidget;
class NodeItem;

/**
 * \file EdgeItem.hpp
 * \class EdgeItem
 * \brief graphical edge representation interface
 * \details used as polymorphic base for several graphical edge implementations
 */
class EdgeItem : public QGraphicsItemGroup
{
public:
    /// empty constructor
    EdgeItem() {}
    /// destructor
    ~EdgeItem() {}

    /**
     * \brief constructor
     * \param graphWidget the parent and managing graph widget
     * \param sourceNode the edge source endpoint vertex
     * \param destNode the edge target endpoint vertex
     * \param edge the internal conceptual edge
     */
    EdgeItem(GraphWidget* graphWidget, NodeItem* sourceNode, NodeItem* destNode, graph_analysis::Edge::Ptr edge);

    /// getter method to retrieve source node
    NodeItem* sourceNodeItem() const;
    /// getter method to retrieve target node
    NodeItem* targetNodeItem() const;

    /// updates edge graphics: edge lenght and endpoints positioning
    virtual void adjust();

    enum { Type = UserType + 2 };
    /// getter method to retrieve constant
    int type() const { return Type; }
    /// getter method to retrieve  the internal conceptual edge
    graph_analysis::Edge::Ptr getEdge() { return mpEdge; }
    // setter method to update the mPainterPath member field
    void setPainterPath(QPainterPath painterPath) { mPainterPath = painterPath; }

    // virtual methods
    virtual EdgeItem* createNewItem(GraphWidget* graphWidget, NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge) const { throw std::runtime_error("graph_analysis::gui::EdgeItem::createNewItem (@4 args) is not reimplemented"); }
    virtual EdgeItem* createNewItem(GraphWidget* graphWidget, NodeItem* sourceNode, int sourceNodePortID, NodeItem* targetNode, int targetNodePortID, graph_analysis::Edge::Ptr edge) const { throw std::runtime_error("graph_analysis::gui::EdgeItem::createNewItem (@6 args) is not reimplemented"); }
    virtual QGraphicsTextItem* getLabel() {  throw std::runtime_error("graph_analysis::gui::EdgeItem: edge label is not implemented at this level"); }
    virtual int getSourcePortID() { throw std::runtime_error("graph_analysis::gui::EdgeItem::getSourcePortID: is not reimplemented at this level"); }
    virtual int getTargetPortID() { throw std::runtime_error("graph_analysis::gui::EdgeItem::getTargetPortID: is not reimplemented at this level"); }
    virtual void releaseFocus()   { throw std::runtime_error("graph_analysis::gui::EdgeItem::releaseFocus is not reimplemented");}
    virtual void adjustLabel()    { throw std::runtime_error("graph_analysis::gui::EdgeItem::adjustLabel is not reimplemented"); }

protected:
    /// retrieves the bounding rectangular box around the area occupied by the node in the scene
    virtual QRectF boundingRect() const;
    /// qt node painting method - here the node components get placed in the scene
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    /// retrieves the path of the bounding rectangular box around the area occupied by the node in the scene
    virtual QPainterPath shape() const;

    /// parent and managing graph widget
    GraphWidget* mpGraphWidget;
    /// source edge-endpoint node
    NodeItem* mpSourceNodeItem;
    /// target edge-endpoint node
    NodeItem* mpTargetNodeItem;
    /// internal conceptual edge on whose basis the edge's been constructed
    graph_analysis::Edge::Ptr mpEdge;

    /// in-scene edge source point
    QPointF mSourcePoint;
    /// in-scene edge target point
    QPointF mTargetPoint;
    /// arrow tip size
    qreal mArrowSize;
    /// qt painter path
    QPainterPath mPainterPath;

    /// maths constants
    static const double Pi;
    static double TwoPi;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_EDGEITEM_HPP
