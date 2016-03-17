#include "Resource.hpp"

#include <QPainter>

#include <base/Logging.hpp>

namespace graph_analysis
{
namespace gui
{
namespace layeritem
{

const std::string Resource::sType("LayerNode");

Resource::Resource(GraphWidget *graphWidget, graph_analysis::Vertex::Ptr vertex)
    : NodeItem(graphWidget, vertex, this)
{
    setFlag(ItemIsMovable);

    mLabel = new QGraphicsTextItem(QString(vertex->getLabel().c_str()), this);
    QFont font = mLabel->font();
    font.setBold(true);
    mLabel->setFont(font);

    mClassName = new QGraphicsTextItem(
        QString(vertex->GraphElement::getClassName().c_str()), this);
    mClassName->setPos(mLabel->pos() +
                       QPoint(0, mLabel->boundingRect().height()));
    mClassName->setDefaultTextColor(Qt::gray);
}

Resource::~Resource()
{
    delete mLabel;
    delete mClassName;
}

QRectF Resource::boundingRect() const
{
    QRectF childrenRect = childrenBoundingRect();
    return childrenRect;
}

QPainterPath Resource::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Resource::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *)
{
    // drawing of border
    painter->setPen(QPen(Qt::blue));
    painter->drawRect(boundingRect());
}

void Resource::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    LOG_DEBUG_S << "Hover ENTER event for " << mpVertex->toString();
    mpGraphWidget->setFocusedElement(mpVertex);
    QGraphicsItem::hoverEnterEvent(event);
}

void Resource::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    LOG_DEBUG_S << "Hover LEAVE event for " << mpVertex->toString();
    mpGraphWidget->clearFocus();
    QGraphicsItem::hoverLeaveEvent(event);
}

} // end namespace layeritem
} // end namespace gui
} // end namespace graph_analysis
