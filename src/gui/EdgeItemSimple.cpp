#include "EdgeItemSimple.hpp"
#include "GraphWidget.hpp"
#include <base-logging/Logging.hpp>
#include <cmath>

namespace graph_analysis {
namespace gui {

EdgeItemSimple::EdgeItemSimple()
    : EdgeItemBase()
    , mpLabel(NULL)
    , mpClassName(NULL)
    , mArrowSize(10)
{
}
// kiss:
EdgeItemSimple::EdgeItemSimple(GraphWidget* graphWidget,
                               const graph_analysis::Edge::Ptr& edge,
                               QGraphicsItem* parent)
    : EdgeItemBase(graphWidget, edge, parent)
    , mpLabel(NULL)
    , mpClassName(NULL)
    , mArrowSize(10)
{
    mpLabel = new QGraphicsTextItem(QString(edge->getLabel().c_str()), this);
    mpClassName = new QGraphicsTextItem("", this);
    mpClassName->setDefaultTextColor(Qt::gray);

    setFlag(ItemIsMovable, false);

}

EdgeItemSimple::~EdgeItemSimple()
{
    delete mpLabel;
    delete mpClassName;
}

int EdgeItemSimple::type() const
{
    return EdgeItemSimpleType;
}

void EdgeItemSimple::adjustEdgePositioning()
{
    prepareGeometryChange();

    drawBezierEdge();
    drawArrowHead(mArrowSize);

    mpLabel->setPos(getEdgePath()->boundingRect().center() -
                    mpLabel->boundingRect().center());
    mpClassName->setPos(mpLabel->pos() +
                        QPointF(0, mpLabel->boundingRect().height()));

}

void EdgeItemSimple::paint(QPainter* painter,
                           const QStyleOptionGraphicsItem* option,
                           QWidget* widget)
{
}

QRectF EdgeItemSimple::boundingRect() const
{
    return childrenBoundingRect();
}

QPainterPath EdgeItemSimple::shape() const
{
    QPainterPath path;
    path = EdgeItemBase::shape() + mpLabel->shape() +
           mpClassName->shape();
    return path;
}

void EdgeItemSimple::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    mpClassName->setPlainText(getEdge()->getClassName().c_str());
}

void EdgeItemSimple::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    mpClassName->setPlainText("");
}


EdgeItemBase* EdgeItemSimple::createNewItem(GraphWidget* graphWidget,
        const graph_analysis::Edge::Ptr& edge,
        QGraphicsItem* parent) const
{
    return new EdgeItemSimple(graphWidget, edge, parent);
}

} // end namespace gui
} // end namespace graph_analysis
