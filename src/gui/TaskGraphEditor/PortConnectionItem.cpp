#include "PortConnectionItem.hpp"

#include <graph_analysis/gui/VertexItemBase.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <base/Logging.hpp>

#include <cmath>

#include <QDebug>

namespace graph_analysis
{
namespace gui
{

PortConnectionItem::PortConnectionItem(GraphWidget *graphWidget,
                                       graph_analysis::task_graph::PortConnection::Ptr edge,
                                       QGraphicsItem *parent)
    : EdgeItemSimple(graphWidget, edge, parent)
{
}

PortConnectionItem::~PortConnectionItem()
{
}

int PortConnectionItem::type() const
{
    return PortConnectionItemType;
}

} // end namespace gui
} // end namespace graph_analysis
