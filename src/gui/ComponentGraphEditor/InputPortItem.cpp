#include "InputPortItem.hpp"

#include <QPen>
#include <QGraphicsSceneMoveEvent>

#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {

// kiss:
InputPortItem::InputPortItem(GraphWidget *graphWidget,
                             graph_analysis::Port::Ptr vertex,
                             QGraphicsItem *parent)
    : PortItem(graphWidget, vertex, parent)
{
    mpLabel = new QGraphicsTextItem(
        QString("in: ") + QString(vertex->getLabel().c_str()), this);
    mpRect = new QGraphicsRectItem(this);
    mpRect->setRect(mpLabel->boundingRect());
    mpRect->setPen(QPen(Qt::red));
    setFlag(ItemIsMovable);
    setZValue(20);
}

InputPortItem::~InputPortItem()
{
    LOG_INFO_S<<parentItem();
    delete mpLabel;
    delete mpRect;
}

void InputPortItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    LOG_INFO_S << "InputPortItem: pressEvent";
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
