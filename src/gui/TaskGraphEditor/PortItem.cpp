#include "PortItem.hpp"

#include <QPen>
#include <QDebug>

#include <base/Logging.hpp>
#include <QGraphicsSceneDragDropEvent>

namespace graph_analysis
{
namespace gui
{

// kiss:
PortItem::PortItem(GraphWidget* graphWidget, graph_analysis::task_graph::Port::Ptr vertex,
                   QGraphicsItem* parent)
    : VertexItemBase(graphWidget, vertex, parent)
    , mpRect(new QGraphicsRectItem(this))
    , mpLabel(new QGraphicsTextItem(this))
{
    mpRect->setPen(QPen(Qt::red));
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

PortItem::~PortItem()
{
    delete mpLabel;
    delete mpRect;
}

} // end namespace gui
} // end namespace graph_analysis
