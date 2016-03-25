#include "InputPortItem.hpp"

#include <QPen>
#include <QGraphicsSceneMoveEvent>
#include <QDebug>

#include <base/Logging.hpp>
#include <graph_analysis/gui/EdgeMimeData.hpp>

namespace graph_analysis {
namespace gui {

// kiss:
InputPortItem::InputPortItem(GraphWidget *graphWidget,
                             graph_analysis::InputPort::Ptr vertex,
                             QGraphicsItem *parent)
    : PortItem(graphWidget, vertex, parent)
{
    updateStrings();
    setAcceptDrops(true);
}

InputPortItem::~InputPortItem()
{
}

void InputPortItem::updateStrings()
{
    mpLabel->setPlainText(QString("in: ") +
                          QString(mpVertex->getLabel().c_str()));
    mpRect->setRect(mpLabel->boundingRect());
}

void InputPortItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    LOG_INFO_S << "InputPortItem: pressEvent";
    // when this event is ignored it will be passed to other item on the same
    // location (of the click)
    event->ignore();
}

void InputPortItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
}

QRectF InputPortItem::boundingRect() const
{
    return childrenBoundingRect();
}

void InputPortItem::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    if(!dynamic_pointer_cast<InputPort>(mpVertex)->isConnected(getGraph()))
    {
        // TODO: more checking, like types and so son...
        // TODO: check that the originating component is not the same component
        // this port is attached to. needs to extract the sharedPtr from
        // mimeData
        event->acceptProposedAction();
        mpRect->setBrush(Qt::green);
    }
    else
    {
        mpRect->setBrush(Qt::red);
    }
}

void InputPortItem::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
    mpRect->setBrush(Qt::NoBrush);
}

void InputPortItem::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    if(!dynamic_pointer_cast<InputPort>(mpVertex)->isConnected(getGraph()))
    {
        // TODO: more checking, like types and so son...
        event->acceptProposedAction();
        mpRect->setBrush(Qt::NoBrush);
        dynamic_cast<const EdgeMimeData*>(event->mimeData())->mpTargetVertex =
            getVertex();
    }
}

} // end namespace gui
} // end namespace graph_analysis
