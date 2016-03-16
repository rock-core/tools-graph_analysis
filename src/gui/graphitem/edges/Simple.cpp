#include "Simple.hpp"
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/items/EdgeLabel.hpp>
#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {
namespace graphitem {
namespace edges {

Simple::Simple(GraphWidget* graphWidget, QGraphicsItem* source, QGraphicsItem* target, graph_analysis::Edge::Ptr edge)
    : EdgeItem(graphWidget, source, target, edge)
    , mpLabel(new items::EdgeLabel(edge->toString(), this)) // the use of edge->toString() is a feature; not a bug!
    , mPenDefault(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin))
    , mFocused(false)
    , mSelected(false)
{
    mPen = mPenDefault; // QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void Simple::setLabel(const std::string& label)
{ 
    mpLabel->setText(label.c_str()); 
}

void Simple::adjust()
{
    if (!mpSourceItem || !mpTargetItem)
    {
        // skipping when one of the endpoints is invalid
        return;
    }

    prepareGeometryChange();


    // Compute center of port nodes
    mSourcePoint = mpSourceItem->scenePos();
    mTargetPoint = mpTargetItem->scenePos();

    // initial complete line
    QLineF line(mSourcePoint, mTargetPoint);
    // adjusting endpoints of the line above
    QPointF intersectionPointWithSource = getIntersectionPoint(mpSourceItem, line);
    QPointF intersectionPointWithTarget = getIntersectionPoint(mpTargetItem, line);

    mLine = QLineF(intersectionPointWithSource, intersectionPointWithTarget);
    adjustLabel();
}

void Simple::adjustLabel()
{
    mpLabel->setPos( mLine.pointAt(0.5) - QPointF(mpLabel->boundingRect().width() / 2., 0) );
}

QRectF Simple::boundingRect() const
{
    if (!mpSourceItem || !mpTargetItem)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + mArrowSize) / 2.0;

    return QRectF(mSourcePoint, QSizeF(mTargetPoint.x() - mSourcePoint.x(),
                                      mTargetPoint.y() - mSourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Simple::paint(QPainter *painter, const QStyleOptionGraphicsItem* options, QWidget*)
{
    if (!mpSourceItem || !mpTargetItem)
    {
        return;
    }

    // Make sure no edge is drawn when endpoint items collide
    if( mpSourceItem->collidesWithItem(mpTargetItem) )
    {
        return;
    }

    // Draw the line itself
    painter->setPen(mPen);
    painter->drawLine(mLine);

    // Draw the arrow(s)
    double angle = ::acos(mLine.dx() / mLine.length());
    if (mLine.dy() >= 0)
        angle = 2*M_PI - angle;

    QPointF targetIntersectionPoint = mLine.pointAt(1);

    QPointF destArrowP1 = targetIntersectionPoint + QPointF(sin(angle - M_PI / 3) * mArrowSize,
                                              cos(angle - M_PI / 3) * mArrowSize);
    QPointF destArrowP2 = targetIntersectionPoint + QPointF(sin(angle - M_PI + M_PI / 3) * mArrowSize,
                                              cos(angle - M_PI + M_PI / 3) * mArrowSize);

    painter->setBrush(mPen.brush());
    painter->drawPolygon(QPolygonF() << mLine.p2() << destArrowP1 << destArrowP2);
}

QPointF Simple::getIntersectionPoint(QGraphicsItem* item, const QLineF& line)
{
    QPolygonF polygon = item->boundingRect();

    // Intersection with target
    QPointF p1 = item->mapToScene(polygon.first());
    QPointF p2;
    QPointF intersectionPoint;
    // iterates through the node boundaries until intersection is found; this fact is guaranteed to happen since one of the endpoints of 'line' lies in the center of the convex body analysed
    for(int i = 1; i < polygon.count(); ++i)
    {
        p2 = item->mapToScene(polygon.at(i));
        QLineF pLine(p1,p2);
        QLineF::IntersectType intersectType =
            pLine.intersect(line, &intersectionPoint);

        if(intersectType == QLineF::BoundedIntersection)
        {
            // intersection found
            break;
        } else {
            // no intersection found
            p1 = p2;
        }
    }
    return intersectionPoint;
}

void Simple::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(!mFocused)
    {
        mPen = QPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    mSelected = true;
    LOG_DEBUG_S << "Hover ENTER event for " << mpEdge->toString();
    QGraphicsItem::hoverEnterEvent(event);
}

void Simple::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(!mFocused)
    {
        mPen = mPenDefault;
    }
    mSelected = false;
    LOG_DEBUG_S << "Hover LEAVE event for " << mpEdge->toString();
    QGraphicsItem::hoverLeaveEvent(event);
}

void Simple::grabFocus()
{
    mPen = QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    update();
    mFocused = true;
}

void Simple::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}

} // end namespace edges
} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis

