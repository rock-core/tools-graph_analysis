#include "EdgeItemBase.hpp"

#include <graph_analysis/gui/VertexItemBase.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <base/Logging.hpp>

#include <cmath>

#include <QDebug>

namespace graph_analysis
{
namespace gui
{

EdgeItemBase::EdgeItemBase(GraphWidget* graphWidget,
                           graph_analysis::Edge::Ptr edge,
                           QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , mpEdge(edge)
    , mpGraphWidget(graphWidget)
{
    // "edges" will not react to mouse-clicks. ever.
    setAcceptedMouseButtons(Qt::NoButton);

    // as long as there is only the simple "straight connection" possible, we
    // check this here. might be moved to later implementations in the future.
    if(edge->getSourceVertex() == edge->getTargetVertex())
    {
        LOG_ERROR_S << "edge " << edge->toString()
                    << " with target and source identical? "
                    << edge->getSourceVertex()->toString();
    }

    setAcceptHoverEvents(true);
}

EdgeItemBase::~EdgeItemBase()
{
}

int EdgeItemBase::type() const
{
    return EdgeItemBaseType;
}

void EdgeItemBase::adjustEdgePoints(QVector<QPointF> points)
{
    mPoints = points;
    adjustEdgePositioning();
}

void EdgeItemBase::adjustEdgePositioning(){}

void EdgeItemBase::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    // set the underlaying edge as focused element
    mpGraphWidget->setFocusedElement(mpEdge);
    QGraphicsItem::hoverEnterEvent(event);
}

void EdgeItemBase::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    mpGraphWidget->clearFocus();
    QGraphicsItem::hoverLeaveEvent(event);
}

// reimplement "shape()" because the default implementation calls
// "boundingRect()" -- we are not rect!
QPainterPath EdgeItemSimple::shape() const
{
    QPainterPath path;
    path = mpMultiLine->shape() + mpArrowHead->shape() + mpLabel->shape() +
           mpClassName->shape();
    return path;
}

// kiss:
EdgeItemSimple::EdgeItemSimple(GraphWidget* graphWidget,
                               graph_analysis::Edge::Ptr edge,
                               QGraphicsItem* parent)
    : EdgeItemBase(graphWidget, edge, parent)
    , mArrowSize(10)
{
    mpLabel = new QGraphicsTextItem(QString(edge->getLabel().c_str()), this);
    mpClassName =
        new QGraphicsTextItem(QString(edge->getClassName().c_str()), this);
    mpClassName->setDefaultTextColor(Qt::gray);
    mpMultiLine = new QGraphicsPolygonItem(this);
    mpMultiLine->setBrush(Qt::NoBrush);
    mpArrowHead = new QGraphicsPolygonItem(this);
    mpArrowHead->setBrush(QBrush(Qt::black));

    setFlag(ItemIsMovable, false);

    mpGraphWidget->registerEdgeItem(mpEdge, this);
}

EdgeItemSimple::~EdgeItemSimple()
{
    delete mpLabel;
    delete mpClassName;
    delete mpMultiLine;
    delete mpArrowHead;

    mpGraphWidget->deregisterEdgeItem(mpEdge, this);
}

int EdgeItemSimple::type() const
{
    return EdgeItemSimpleType;
}

void EdgeItemSimple::adjustEdgePositioning()
{
    prepareGeometryChange();

    mpMultiLine->setPolygon(mPoints);
    mpLabel->setPos(mpMultiLine->boundingRect().center() -
                    mpLabel->boundingRect().center());
    mpClassName->setPos(mpLabel->pos() +
                        QPointF(0, mpLabel->boundingRect().height()));

    // draw the arrow!
    QLineF lastSegment(mPoints.at(mPoints.size()-2), mPoints.at(mPoints.size()-1));
    double angle = std::acos(lastSegment.dx() / lastSegment.length());
    if(lastSegment.dy() >= 0)
        angle = 2 * M_PI - angle;

    QPointF destArrowP1 =
        mPoints.last() + QPointF(sin(angle - M_PI / 3) * mArrowSize,
                                          cos(angle - M_PI / 3) * mArrowSize);
    QPointF destArrowP2 = mPoints.last() +
                          QPointF(sin(angle - M_PI + M_PI / 3) * mArrowSize,
                                  cos(angle - M_PI + M_PI / 3) * mArrowSize);
    mpArrowHead->setPolygon(QPolygonF() << lastSegment.p2() << destArrowP1
                                        << destArrowP2);
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

} // end namespace gui
} // end namespace graph_analysis
