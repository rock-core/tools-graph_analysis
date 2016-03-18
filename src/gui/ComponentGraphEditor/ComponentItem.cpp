#include "ComponentItem.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <base/Logging.hpp>
#include <graph_analysis/gui/items/Feature.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/BaseGraphView/BaseGraphView.hpp>

namespace graph_analysis {
namespace gui {

// kiss:
ComponentItem::ComponentItem(GraphWidget *graphWidget,
                                   graph_analysis::Component::Ptr vertex,
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

ComponentItem::~ComponentItem()
{
    delete mpLabel;
    delete mpClassName;
    delete mpRect;
}

} // end namespace gui
} // end namespace graph_analysis
