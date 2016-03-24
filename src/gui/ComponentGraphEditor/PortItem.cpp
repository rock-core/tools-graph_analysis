#include "PortItem.hpp"

#include <QPen>

#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {

// kiss:
PortItem::PortItem(GraphWidget *graphWidget, graph_analysis::Port::Ptr vertex,
                   QGraphicsItem *parent)
    : VertexItemBase(graphWidget, vertex, parent),
      mpLabel(new QGraphicsTextItem(this)), mpRect(new QGraphicsRectItem(this))
{
    mpRect->setPen(QPen(Qt::red));
}

PortItem::~PortItem()
{
    delete mpLabel;
    delete mpRect;
}


} // end namespace gui
} // end namespace graph_analysis
