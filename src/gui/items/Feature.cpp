#include "Feature.hpp"
#include <QDebug>

#include <QMenu>
#include <exception>
#include <boost/lexical_cast.hpp>
#include <base-logging/Logging.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis {
namespace gui {
namespace items {

Feature::Feature(GraphElement::Ptr element, GraphWidget *graphWidget, QGraphicsItem* parent)
    : QGraphicsTextItem(element->getLabel().c_str(), parent)
    , mpGraphWidget(graphWidget)
    , mpGraphElement(element)
{
    setAcceptHoverEvents(true);
    setAcceptDrops(true);
}


Edge::Ptr Feature::getEdge() const
{
    Edge::Ptr edge = dynamic_pointer_cast<Edge>(mpGraphElement);
    if(!edge)
    {
        throw std::runtime_error("graph_analysis::gui::items::Feature::getEdge: feature is no associated with an edge");
    }
    return edge;
}

Vertex::Ptr Feature::getVertex() const
{
    Vertex::Ptr vertex = dynamic_pointer_cast<Vertex>(mpGraphElement);
    if(!vertex)
    {
        throw std::runtime_error("graph_analysis::gui::items::Feature::getVertex: feature is no associated with an vertex");
    }
    return vertex;
}

void Feature::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void Feature::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QRectF rect = boundingRect();

    painter->setPen(Qt::red);
    painter->drawRoundedRect(rect, 10, 10);

    QGraphicsTextItem::paint(painter, option, widget);
}

} // end namespace items
} // end namespace gui
} // end namespace graph_analysis
