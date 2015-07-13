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

#ifndef GRAPH_ANALYSIS_GUI_NODEITEM_HPP
#define GRAPH_ANALYSIS_GUI_NODEITEM_HPP

#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QList>
#include <set>
#include <vector>
#include <string>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/gui/graphitem/Label.hpp>

#define LABEL_SWAPPING

namespace graph_analysis {
namespace gui {

namespace graphitem {
    class Label;
} // end namespace graphitem

class EdgeItem;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class NodeItem : public QGraphicsItemGroup
{
public:
    typedef long long portID_t;
    typedef std::map<portID_t, graphitem::Label*> Labels;
    typedef std::pair<portID_t, graphitem::Label*> Tuple;
    typedef std::map<portID_t, graph_analysis::Vertex::Ptr> Vertices;
    typedef std::pair<portID_t, graph_analysis::Vertex::Ptr> VTuple;
protected:
    NodeItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);

public:
    NodeItem() {}

    virtual ~NodeItem() {};

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    void calculateForces();
    bool advance();
    virtual QPolygonF portBoundingPolygon   (portID_t portID)  { throw std::runtime_error("graph_analysis::gui::NodeItem::portBoundingPolygon is not reimplemented");    }
    virtual QRectF    portBoundingRect      (portID_t portID)  { throw std::runtime_error("graph_analysis::gui::NodeItem::portBoundingRect is not reimplemented");       }
    virtual void setPortLabel(portID_t portID, const std::string& label) { throw std::runtime_error("graph_analysis::gui::NodeItem::setPortLabel is not reimplemented"); }
    virtual void unselect() { throw std::runtime_error("graph_analysis::gui::NodeItem::unselect is not reimplemented"); }

    /**
     * Get the center position of this node item
     * The default implementation uses the boundingRect() and pos() to compute this information
     * \return center position of this node item in the scene
     */
    virtual QPointF getCenterPosition() const;
    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { throw std::runtime_error("graph_analysis::gui::NodeItem::createNewItem is not reimplemented"); }

    std::string getId() const;
    graph_analysis::Vertex::Ptr getVertex() { return mpVertex; }
    void setVertex(graph_analysis::Vertex::Ptr vertex) { mpVertex = vertex; }
    GraphWidget* getGraphWidget() { return mpGraphWidget; }

    virtual void changeLabel(const std::string& label)  { throw std::runtime_error("graph_analysis::gui::NodeItem::changeLabel is not reimplemented");  }
    virtual void updateLabel()                          { throw std::runtime_error("graph_analysis::gui::NodeItem::updateLabel is not reimplemented");  }
    virtual void setPortCount(unsigned)                 { throw std::runtime_error("graph_analysis::gui::NodeItem::setPortCount is not reimplemented"); }
    virtual unsigned  getPortCount()                    { throw std::runtime_error("graph_analysis::gui::NodeItem::getPortCount is not reimplemented"); }
    virtual portID_t  addPort(Vertex::Ptr)              { throw std::runtime_error("graph_analysis::gui::NodeItem::addPort is not reimplemented");      }
    virtual void removePort(portID_t portID)            { throw std::runtime_error("graph_analysis::gui::NodeItem::removePort is not reimplemented");   }
    virtual graph_analysis::Vertex::Ptr getPort(portID_t portID)    {throw std::runtime_error("graph_analysis::gui::NodeItem::getPort is not reimplemented");}
    virtual std::string  getLabel()                     { throw std::runtime_error("graph_analysis::gui::NodeItem::getLabel is not reimplemented");     }
    virtual void syncLabel(portID_t portID)             { throw std::runtime_error("graph_analysis::gui::NodeItem::syncLabel is not reimplemented");    }
    virtual void shiftPortUp(portID_t portID)           { throw std::runtime_error("graph_analysis::gui::NodeItem::shiftPortUp is not reimplemented");  }
    virtual void shiftPortDown(portID_t portID)         { throw std::runtime_error("graph_analysis::gui::NodeItem::shiftPortDown is not reimplemented");}
    virtual void prepareChange()                        { throw std::runtime_error("graph_analysis::gui::NodeItem::prepareChange is not reimplemented");}
    virtual void releaseFocus ()                        { throw std::runtime_error("graph_analysis::gui::NodeItem::releaseFocus is not reimplemented"); }
    virtual void updateWidth  ()                        { throw std::runtime_error("graph_analysis::gui::NodeItem::updateWidth  is not reimplemented"); }
    virtual void updateHeight ()                        { throw std::runtime_error("graph_analysis::gui::NodeItem::updateHeight is not reimplemented"); }
    virtual void swapPorts(portID_t, portID_t)          { throw std::runtime_error("graph_analysis::gui::NodeItem::swapPorts is not reimplemented");    }
    virtual void removePorts()                          { throw std::runtime_error("graph_analysis::gui::NodeItem::removePorts is not reimplemented");  }
    virtual Labels      getLabels()     { throw std::runtime_error("graph_analysis::gui::NodeItem::getLabels is not reimplemented");   }
    virtual Vertices    getVertices()   { throw std::runtime_error("graph_analysis::gui::NodeItem::getVertices is not reimplemented"); }


protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);

    graph_analysis::Vertex::Ptr mpVertex;
    QPointF mNewPos;
    GraphWidget* mpGraphWidget;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_NODEITEM_HPP
