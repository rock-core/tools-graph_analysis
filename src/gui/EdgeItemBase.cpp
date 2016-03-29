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

// edge-routing mit graphviz: https://github.com/jmachowinski/qgv

EdgeItemBase::EdgeItemBase(GraphWidget* graphWidget,
                           graph_analysis::Edge::Ptr edge,
                           VertexItemBase* sourceItem,
                           VertexItemBase* targetItem, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , mpEdge(edge)
    , mpGraphWidget(graphWidget)
    , mpSourceItem(sourceItem)
    , mpTargetItem(targetItem)
{
    // "edges" will not react to mouse-clicks. ever.
    setAcceptedMouseButtons(Qt::NoButton);

    // as lone as there is only the simple "straight connection" possible, we
    // check this here. might be moved to later implementations in the future.
    if(edge->getSourceVertex() == edge->getTargetVertex())
    {
        LOG_ERROR_S << "edge " << edge->toString()
                    << " with target and source identical? "
                    << edge->getSourceVertex()->toString();
    }

    // tell the two vertices we are connected to that we want to be updated
    // when their position changes
    //
    // problem: what if these are no longer member of the scene?
    //
    // IDEA: how about creating the connected edge via a QGraphicsItemGroup
    // containing the two ports (which are also contained elsewhere) plus the
    // actual edge?
    mpSourceItem->registerConnection(this);
    mpTargetItem->registerConnection(this);
}

EdgeItemBase::~EdgeItemBase()
{
    // this can crash during tearing down of a scene because the pointers seize
    // to be valid. guarding against this the expensive way...
    if (scene()->items().contains(mpSourceItem))
        mpSourceItem->deregisterConnection(this);
    if (scene()->items().contains(mpTargetItem))
        mpTargetItem->deregisterConnection(this);
}

void EdgeItemBase::adjust()
{
    mSourcePoint = mpSourceItem->getConnector();
    mTargetPoint = mpTargetItem->getConnector();

    prepareGeometryChange();
}

void EdgeItemBase::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // Set the underlaying edge as focused element
    mpGraphWidget->setFocusedElement(mpEdge);
    QGraphicsItem::hoverEnterEvent(event);
}

void EdgeItemBase::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mpGraphWidget->clearFocus();
    QGraphicsItem::hoverLeaveEvent(event);
}

// reimplement "shape()" because the default implementation calls
// "boundingRect()" -- we are not rect!
QPainterPath EdgeItemSimple::shape() const
{
    QPainterPath path;
    path = mpLine->shape() + mpArrowHead->shape() + mpLabel->shape() +
           mpClassName->shape();
    return path;
}

// kiss:
EdgeItemSimple::EdgeItemSimple(GraphWidget *graphWidget,
                               graph_analysis::Edge::Ptr edge,
                               VertexItemBase *source, VertexItemBase *target,
                               QGraphicsItem *parent)
    : EdgeItemBase(graphWidget, edge, source, target, parent), mArrowSize(10)
{
    mpLabel = new QGraphicsTextItem(QString(edge->getLabel().c_str()), this);
    mpClassName = new QGraphicsTextItem(QString(edge->getClassName().c_str()), this);
    mpClassName->setDefaultTextColor(Qt::gray);
    mpLine = new QGraphicsLineItem(this);
    mpArrowHead = new QGraphicsPolygonItem(this);
    mpArrowHead->setBrush(QBrush(Qt::black));

    setFlag(ItemIsMovable, false);

    mpGraphWidget->registerEdgeItem(mpEdge, this);
}

EdgeItemSimple::~EdgeItemSimple()
{
    delete mpLabel;
    delete mpClassName;
    delete mpLine;
    delete mpArrowHead;

    mpGraphWidget->deregisterEdgeItem(mpEdge, this);
}

QPointF EdgeItemSimple::getIntersectionPoint(QGraphicsItem *item) const
{
    QPolygonF polygon = item->boundingRect();

    // Intersection with target
    QPointF p1 = item->mapToScene(polygon.first());
    QPointF p2;
    QPointF intersectionPoint;
    // iterates through the node boundaries until intersection is found; this
    // fact is guaranteed to happen since one of the endpoints of 'line' lies in
    // the center of the convex body analysed
    //
    // FIXME: catch the "no intersection" case
    for(int i = 1; i < polygon.count(); ++i)
    {
        p2 = item->mapToScene(polygon.at(i));
        QLineF pLine(p1, p2);
        QLineF::IntersectType intersectType =
            pLine.intersect(mpLine->line(), &intersectionPoint);

        if(intersectType == QLineF::BoundedIntersection)
        {
            // intersection found
            break;
        }
        else
        {
            // no intersection found
            p1 = p2;
        }
    }
    return intersectionPoint;
}

void EdgeItemSimple::adjust()
{
    EdgeItemBase::adjust();

    mpLine->setLine(QLineF(mSourcePoint, mTargetPoint));

    mSourcePoint = getIntersectionPoint(mpSourceItem);
    mTargetPoint = getIntersectionPoint(mpTargetItem);

    mpLine->setLine(QLineF(mSourcePoint, mTargetPoint));
    mpLabel->setPos(mpLine->line().pointAt(0.5) -
                    mpLabel->boundingRect().center());
    mpClassName->setPos(mpLabel->pos() +
                    QPointF(0, mpLabel->boundingRect().height()));

    // draw the arrow!
    double angle = std::acos(mpLine->line().dx() / mpLine->line().length());
    if(mpLine->line().dy() >= 0)
        angle = 2 * M_PI - angle;

    QPointF targetIntersectionPoint = mpLine->line().pointAt(1);

    QPointF destArrowP1 =
        targetIntersectionPoint + QPointF(sin(angle - M_PI / 3) * mArrowSize,
                                          cos(angle - M_PI / 3) * mArrowSize);
    QPointF destArrowP2 = targetIntersectionPoint +
                          QPointF(sin(angle - M_PI + M_PI / 3) * mArrowSize,
                                  cos(angle - M_PI + M_PI / 3) * mArrowSize);
    mpArrowHead->setPolygon(QPolygonF() << mpLine->line().p2() << destArrowP1
                                        << destArrowP2);
}

void EdgeItemSimple::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
}

QRectF EdgeItemSimple::boundingRect() const { return childrenBoundingRect(); }

} // end namespace gui
} // end namespace graph_analysis
