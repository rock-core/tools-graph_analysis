#include "EdgeItemBase.hpp"

#include <graph_analysis/gui/VertexItemBase.hpp>
#include <base/Logging.hpp>

#include <QDebug>

namespace graph_analysis
{
namespace gui
{

EdgeItemBase::EdgeItemBase(GraphWidget *graphWidget,
                           graph_analysis::Edge::Ptr edge,
                           VertexItemBase *source, VertexItemBase *target,
                           QGraphicsItem *parent)
    : QGraphicsItemGroup(parent), mpEdge(edge), mpGraphWidget(graphWidget),
      mpSource(source), mpTarget(target)
{
    setAcceptedMouseButtons(Qt::NoButton);

    if(!edge->getSourceVertex())
    {
        LOG_ERROR_S << "no source in " << edge->getLabel() << "?";
    }
    if(!edge->getTargetVertex())
    {
        LOG_ERROR_S << "no target in " << edge->getLabel() << "?";
    }
}

/* QRectF EdgeItemBase::boundingRect() const */
/* { */
/*     return QRectF(mSourcePoint, mTargetPoint); */
/* } */

void EdgeItemBase::adjust()
{
    mSourcePoint =
        mapToScene(mpSource->pos() + mpSource->boundingRect().center());
    mTargetPoint =
        mapToScene(mpTarget->pos() + mpTarget->boundingRect().center());

    prepareGeometryChange();
}

void EdgeItemBase::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // Set the underlaying edge as focused element
    mpGraphWidget->setFocusedElement(mpEdge);
    QGraphicsItemGroup::hoverEnterEvent(event);
}

void EdgeItemBase::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mpGraphWidget->clearFocus();
    QGraphicsItemGroup::hoverLeaveEvent(event);
}

QRectF EdgeItemBase::boundingRect() const
{
    return childrenBoundingRect();
}

// kiss:
EdgeItemSimple::EdgeItemSimple(GraphWidget *graphWidget,
                               graph_analysis::Edge::Ptr edge,
                               VertexItemBase *source, VertexItemBase *target,
                               QGraphicsItem *parent)
    : EdgeItemBase(graphWidget, edge, source, target, parent)
{
    mLabel = new QGraphicsTextItem(QString(edge->getLabel().c_str()), this);
    mLine = new QGraphicsLineItem(this);

    setFlag(ItemIsMovable, false);

    adjust();
}

EdgeItemSimple::~EdgeItemSimple()
{
    delete mLabel;
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
            pLine.intersect(mLine->line(), &intersectionPoint);

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

    mLine->setLine(QLineF(mSourcePoint, mTargetPoint));

    mSourcePoint = getIntersectionPoint(mpSource);
    mTargetPoint = getIntersectionPoint(mpTarget);

    mLine->setLine(QLineF(mSourcePoint, mTargetPoint));
    mLabel->setPos(mLine->line().pointAt(0.5)-mLabel->boundingRect().center());
}

void EdgeItemSimple::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    // draw the arrow!
    double angle = std::acos(mLine.line().dx() / mLine.line().length());
    if (mLine.line().dy() >= 0)
        angle = 2*M_PI - angle;

    QPointF targetIntersectionPoint = mLine.line().pointAt(1);

    QPointF destArrowP1 = targetIntersectionPoint + QPointF(sin(angle - M_PI / 3) * mArrowSize,
                                              cos(angle - M_PI / 3) * mArrowSize);
    QPointF destArrowP2 = targetIntersectionPoint + QPointF(sin(angle - M_PI + M_PI / 3) * mArrowSize,
                                              cos(angle - M_PI + M_PI / 3) * mArrowSize);

    painter->setBrush(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPolygon(QPolygonF() << mLine.p2() << destArrowP1 << destArrowP2);
}

} // end namespace gui
} // end namespace graph_analysis
