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

QRectF VertexItemBase::boundingRect() const
{
    return childrenBoundingRect();
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

QVariant VertexItemBase::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch (change)
    {
        case ItemPositionHasChanged:
        {
            //FIXME: proper API
            dynamic_cast<BaseGraphView *>(mpGraphWidget)->adjustEdgesOf(this);
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
        QString(vertex->GraphElement::getClassName().c_str()), this);
    mpClassName->setPos(mpLabel->pos() +
                       QPoint(0, mpLabel->boundingRect().height()));
    mpClassName->setDefaultTextColor(Qt::gray);

    setFlag(ItemIsMovable);
}

VertexItemSimple::~VertexItemSimple()
{
    delete mpLabel;
    delete mpClassName;
}

void VertexItemSimple::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option, QWidget *)
{
    // drawing of border
    painter->setPen(QPen(Qt::blue));
    painter->drawRect(boundingRect());
}

} // end namespace gui
} // end namespace graph_analysis
