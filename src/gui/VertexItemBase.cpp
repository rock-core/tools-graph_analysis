#include "VertexItemBase.hpp"

#include "GraphWidget.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <base/Logging.hpp>
#include <graph_analysis/gui/items/Feature.hpp>

namespace graph_analysis {
namespace gui {

VertexItemBase::VertexItemBase(GraphWidget *graphWidget,
                               graph_analysis::Vertex::Ptr vertex,
                               QGraphicsItem *parent)
    : QGraphicsItemGroup(parent), mpVertex(vertex), mpGraphWidget(graphWidget)
{
    // this enabled "itemChange()" notifications. when this item moves, it has
    // to tell its edges to follow it, so they stay visually connected. this is
    // done by calling "adjust()" for the respective edge
    setFlag(ItemSendsGeometryChanges);
    // this cache-mode is for items that can move. not sure if we can move --
    // edges can move?
    setCacheMode(DeviceCoordinateCache);
}

void VertexItemBase::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    // Set the underlaying vertex as focused element
    mpGraphWidget->setFocusedElement(mpVertex);
    QGraphicsItemGroup::hoverEnterEvent(event);
}

void VertexItemBase::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    mpGraphWidget->clearFocus();
    QGraphicsItemGroup::hoverLeaveEvent(event);
}

} // end namespace gui
} // end namespace graph_analysis
