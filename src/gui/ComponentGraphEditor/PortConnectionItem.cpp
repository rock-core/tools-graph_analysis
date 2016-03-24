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
                                       graph_analysis::PortConnection::Ptr edge,
                                       VertexItemBase *source,
                                       VertexItemBase *target,
                                       QGraphicsItem *parent)
    : EdgeItemSimple(graphWidget, edge, source, target, parent)
{
}

PortConnectionItem::~PortConnectionItem()
{
}

} // end namespace gui
} // end namespace graph_analysis
