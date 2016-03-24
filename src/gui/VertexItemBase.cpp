#include "VertexItemBase.hpp"

#include "GraphWidget.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <base/Logging.hpp>
#include <graph_analysis/gui/items/Feature.hpp>
#include <graph_analysis/gui/BaseGraphView/BaseGraphView.hpp>

namespace graph_analysis {
namespace gui {

VertexItemBase::VertexItemBase(GraphWidget *graphWidget,
                               graph_analysis::Vertex::Ptr vertex,
                               QGraphicsItem *parent)
    : QGraphicsItem(parent), mpVertex(vertex), mpGraphWidget(graphWidget)
{
    // this enabled "itemChange()" notifications. when this item moves, it has
    // to tell its edges to follow it, so they stay visually connected. this is
    // done by calling "adjust()" for the respective edge
    setFlag(ItemSendsGeometryChanges);
    // this cache-mode is for items that can move. not sure if we can move --
    // edges can move?
    setCacheMode(DeviceCoordinateCache);

    mpGraphWidget->registerVertexItem(mpVertex, this);
    LOG_INFO_S<<"registered vertex " << this << " " << mpVertex << " " << mpVertex->getClassName();
}

VertexItemBase::~VertexItemBase()
{
    mpGraphWidget->deregisterVertexItem(mpVertex, this);
}

void VertexItemBase::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    // Set the underlaying vertex as focused element
    mpGraphWidget->setFocusedElement(mpVertex);
    QGraphicsItem::hoverEnterEvent(event);
}

void VertexItemBase::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    mpGraphWidget->clearFocus();
    QGraphicsItem::hoverLeaveEvent(event);
}

void VertexItemBase::registerConnection(EdgeItemBase *item)
{
    adjustConnections.push_back(item);
}

void VertexItemBase::deregisterConnection(EdgeItemBase *item)
{
    int index = adjustConnections.indexOf(item);
    if(index != -1)
    {
        adjustConnections.remove(index);
    }
    else
    {
        LOG_ERROR_S << "ba";
    }
}

QVariant VertexItemBase::itemChange(GraphicsItemChange change,
                                    const QVariant& value)
{
    switch(change)
    {
    case ItemPositionHasChanged:
    {
        for(int i = 0; i < adjustConnections.size(); ++i)
        {
            adjustConnections.at(i)->adjust();
        }
        break;
    }
    default:
        break;
    };
    return QGraphicsItem::itemChange(change, value);
}

// kiss:
VertexItemSimple::VertexItemSimple(GraphWidget *graphWidget,
                                   graph_analysis::Vertex::Ptr vertex,
                                   QGraphicsItem *parent)
    : VertexItemBase(graphWidget, vertex, parent)
{
    mpLabel = new QGraphicsTextItem(QString(vertex->getLabel().c_str()), this);
    QFont font = mpLabel->font();
    font.setBold(true);
    mpLabel->setFont(font);

    mpClassName = new QGraphicsTextItem(
        QString(vertex->getClassName().c_str()), this);
    mpClassName->setPos(mpLabel->pos() +
                       QPoint(0, mpLabel->boundingRect().height()));
    mpClassName->setDefaultTextColor(Qt::gray);

    mpRect = new QGraphicsRectItem(this);
    mpRect->setRect(childrenBoundingRect());
    mpRect->setPen(QPen(Qt::blue));

    setFlag(ItemIsMovable);
}

VertexItemSimple::~VertexItemSimple()
{
    delete mpLabel;
    delete mpClassName;
    delete mpRect;
}

void VertexItemSimple::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
}

QRectF VertexItemSimple::boundingRect() const { return childrenBoundingRect(); }

} // end namespace gui
} // end namespace graph_analysis
