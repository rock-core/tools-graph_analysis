#include "PortConnectionItem.hpp"

#include <graph_analysis/gui/VertexItemBase.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <base/Logging.hpp>

#include <cmath>

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

namespace graph_analysis
{
namespace gui
{

PortConnectionItem::PortConnectionItem(GraphWidget *graphWidget,
                                       graph_analysis::task_graph::PortConnection::Ptr edge,
                                       QGraphicsItem *parent)
    : EdgeItemSimple(graphWidget, edge, parent)
{
    setAcceptedMouseButtons(Qt::RightButton);
}

PortConnectionItem::~PortConnectionItem()
{
}

int PortConnectionItem::type() const
{
    return PortConnectionItemType;
}

void PortConnectionItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->button() == Qt::RightButton)
    {
        // Delete myself oO
        BaseGraph::Ptr graph = mpGraphWidget->graph();
        graph_analysis::task_graph::PortConnection::Ptr edge = dynamic_pointer_cast<graph_analysis::task_graph::PortConnection>(mpEdge);
        graph->removeEdge(edge);
    }
}

} // end namespace gui
} // end namespace graph_analysis
