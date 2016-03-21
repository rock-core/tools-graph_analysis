#include "InputPortItem.hpp"

#include <QPen>
#include <QGraphicsSceneMoveEvent>

#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {

// kiss:
InputPortItem::InputPortItem(GraphWidget *graphWidget,
                             graph_analysis::InputPort::Ptr vertex,
                             QGraphicsItem *parent)
    : PortItem(graphWidget, vertex, parent)
{
    updateStrings();
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

QRectF InputPortItem::boundingRect() const { return childrenBoundingRect(); }

} // end namespace gui
} // end namespace graph_analysis
