#include "VertexItemBase.hpp"

#include "GraphWidget.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include <base-logging/Logging.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis {
namespace gui {

VertexItemBase::VertexItemBase()
    : QGraphicsItem(NULL)
    , QGraphicsLayoutItem()
    , mpVertex()
    , mpGraphWidget(NULL)
{}

VertexItemBase::VertexItemBase(GraphWidget* graphWidget,
                               graph_analysis::Vertex::Ptr vertex,
                               QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , QGraphicsLayoutItem()
    , mpVertex(vertex)
    , mpGraphWidget(graphWidget)
{
    // this enabled "itemChange()" notifications. when this item moves, it has
    // to tell its edges to follow it, so they stay visually connected. this is
    // done by calling "adjust()" for the respective edge
    setFlag(ItemSendsScenePositionChanges);
    // this cache-mode is for items that can move. not sure if we can move --
    // vertices can move?
    setCacheMode(DeviceCoordinateCache);
    // with this we tell the graphwidget that "this" is responsible to
    // represent "mpVertex"
    //
    // shouldn't this be moved elsewhere?
    mpGraphWidget->registerVertexItem(mpVertex, this);
    // drag'n drop is used to create edges between vertices
    setAcceptHoverEvents(true);

    // for QGraphicsLayoutItem
    setGraphicsItem(this);
}

VertexItemBase::~VertexItemBase()
{
    mpGraphWidget->deregisterVertexItem(mpVertex, this);
}

BaseGraph::Ptr VertexItemBase::getGraph() const
{
    return mpGraphWidget->graph();
}

void VertexItemBase::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    // set the underlaying vertex as focused element in the statusbar
    mpGraphWidget->setFocusedElement(mpVertex);
    QGraphicsItem::hoverEnterEvent(event);
}

void VertexItemBase::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    // set focused element in the statusbar
    mpGraphWidget->clearFocus();
    QGraphicsItem::hoverLeaveEvent(event);
}

QVariant VertexItemBase::itemChange(GraphicsItemChange change,
                                    const QVariant& value)
{
    switch(change)
    {
    case ItemScenePositionHasChanged:
    {
        // notify the graph widget about our new position. there, relevant
        // caching and updating of connected items is performed.
        getGraphWidget()->vertexPositionHasChanged(this);
        break;
    }
    default:
    {
        break;
    }
    };
    return QGraphicsItem::itemChange(change, value);
}

void VertexItemBase::setGeometry(const QRectF& geometry)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(geometry);
    setPos(geometry.topLeft());
}

QSizeF VertexItemBase::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    switch (which) {
        case Qt::MinimumSize:
        case Qt::PreferredSize:
            // Do not allow a size smaller than 30x30
            return QSize(30, 30);
        case Qt::MaximumSize:
            return QSizeF(1000,1000);
        default:
            break;
    }
    return constraint;
}

void VertexItemBase::paint(QPainter* painter,
                             const QStyleOptionGraphicsItem* option,
                             QWidget* widget)
{
}

QRectF VertexItemBase::boundingRect() const
{
    return childrenBoundingRect();
}

} // end namespace gui
} // end namespace graph_analysis
