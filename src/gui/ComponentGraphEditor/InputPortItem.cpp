#include "InputPortItem.hpp"

#include <QPen>

namespace graph_analysis {
namespace gui {

// kiss:
InputPortItem::InputPortItem(GraphWidget *graphWidget,
                             graph_analysis::InputPort::Ptr vertex,
                             QGraphicsItem *parent)
    : VertexItemBase(graphWidget, vertex, parent)
{
    mpLabel = new QGraphicsTextItem(QString("in: ")+QString(vertex->getLabel().c_str()), this);
    mpRect = new QGraphicsRectItem(this);
    mpRect->setRect(mpLabel->boundingRect());
    mpRect->setPen(QPen(Qt::red));
}

InputPortItem::~InputPortItem()
{
    delete mpLabel;
    delete mpRect;
}

} // end namespace gui
} // end namespace graph_analysis
