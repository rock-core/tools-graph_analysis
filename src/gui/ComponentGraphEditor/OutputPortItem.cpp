#include "OutputPortItem.hpp"

#include <QPen>
#include <QGraphicsSceneMoveEvent>

#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {

// kiss:
OutputPortItem::OutputPortItem(GraphWidget *graphWidget,
                             graph_analysis::OutputPort::Ptr vertex,
                             QGraphicsItem *parent)
    : PortItem(graphWidget, vertex, parent)
{
    mpLabel = new QGraphicsTextItem(QString("out: ")+QString(vertex->getLabel().c_str()), this);
    mpRect = new QGraphicsRectItem(this);
    mpRect->setRect(mpLabel->boundingRect());
    mpRect->setPen(QPen(Qt::red));
}

OutputPortItem::~OutputPortItem()
{
    delete mpLabel;
    delete mpRect;
}

void OutputPortItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    LOG_INFO_S << "OutputPortItem: pressEvent";
    event->ignore();
}

void OutputPortItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
}

QRectF OutputPortItem::boundingRect() const { return childrenBoundingRect(); }

} // end namespace gui
} // end namespace graph_analysis
