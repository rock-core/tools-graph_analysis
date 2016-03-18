#include "OutputPortItem.hpp"

#include <QPen>

namespace graph_analysis {
namespace gui {

// kiss:
OutputPortItem::OutputPortItem(GraphWidget *graphWidget,
                             graph_analysis::OutputPort::Ptr vertex,
                             QGraphicsItem *parent)
    : VertexItemBase(graphWidget, vertex, parent)
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

} // end namespace gui
} // end namespace graph_analysis
