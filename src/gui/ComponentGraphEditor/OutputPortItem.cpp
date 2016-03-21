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
    updateStrings();
}

OutputPortItem::~OutputPortItem()
{
}

void OutputPortItem::updateStrings()
{
    LOG_INFO_S<<mpRect->parentItem();
    mpLabel->setPlainText(QString("in: ") +
                          QString(mpVertex->getLabel().c_str()));
    mpRect->setRect(mpLabel->boundingRect());
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
