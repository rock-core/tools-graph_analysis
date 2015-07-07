#include "Simple.hpp"
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/NodeItem.hpp>

namespace graph_analysis {
namespace gui {
namespace graphitem {
namespace edges {

Simple::Simple(GraphWidget* graphWidget, NodeItem* sourceNode, int sourceNodePortID, NodeItem* targetNode, int targetNodePortID, graph_analysis::Edge::Ptr edge)
    : EdgeItem(graphWidget, sourceNode, targetNode, edge)
    , mpLabel(new EdgeLabel(edge->toString(), this)) // the use of edge->toString() is a feature; not a bug!
    , mPenDefault(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin))
    , mSourceNodePortID(sourceNodePortID)
    , mTargetNodePortID(targetNodePortID)
    , mFocused(false)
    , mSelected(false)
{
    mPen = mPenDefault; // QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void Simple::adjust()
{
    if (!mpSourceNodeItem || !mpTargetNodeItem)
        return;

    prepareGeometryChange();

    // Compute center of port nodes
    mSourcePoint = mpSourceNodeItem->mapToScene(mpSourceNodeItem->portBoundingRect(mSourceNodePortID).center());
    mTargetPoint = mpTargetNodeItem->mapToScene(mpTargetNodeItem->portBoundingRect(mTargetNodePortID).center());

//    QPointF centerPos((mTargetPoint.x() - mSourcePoint.x())/2.0, (mTargetPoint.y() - mSourcePoint.y())/2.0);

    QLineF line(mSourcePoint, mTargetPoint);
    QPointF intersectionPointWithSource = getIntersectionPoint(mpSourceNodeItem, line, mSourceNodePortID);
    QPointF intersectionPointWithTarget = getIntersectionPoint(mpTargetNodeItem, line, mTargetNodePortID);

    mLine = QLineF(intersectionPointWithSource, intersectionPointWithTarget);
    mpLabel->setPos( mLine.pointAt(0.5) - QPointF(mpLabel->boundingRect().width() / 2., 0) );
}

QRectF Simple::boundingRect() const
{
    if (!mpSourceNodeItem || !mpTargetNodeItem)
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
    if (!mpSourceNodeItem || !mpTargetNodeItem)
    {
        return;
    }

    // Make sure no edge is drawn when items collide
    if( mpSourceNodeItem->collidesWithItem(mpTargetNodeItem) )
    {
        return;
    }

    // Draw the line itself
    painter->setPen(mPen);
    painter->drawLine(mLine);

    // Draw the arrows
    double angle = ::acos(mLine.dx() / mLine.length());
    if (mLine.dy() >= 0)
        angle = TwoPi - angle;

    QPointF targetIntersectionPoint = mLine.pointAt(1);

    QPointF destArrowP1 = targetIntersectionPoint + QPointF(sin(angle - Pi / 3) * mArrowSize,
                                              cos(angle - Pi / 3) * mArrowSize);
    QPointF destArrowP2 = targetIntersectionPoint + QPointF(sin(angle - Pi + Pi / 3) * mArrowSize,
                                              cos(angle - Pi + Pi / 3) * mArrowSize);

    painter->setBrush(mPen.brush());
    painter->drawPolygon(QPolygonF() << mLine.p2() << destArrowP1 << destArrowP2);
}

QPointF Simple::getIntersectionPoint(NodeItem* item, const QLineF& line, int portID)
{
    QPolygonF polygon = (-1 == portID) ? item->boundingRect() : item->portBoundingPolygon(portID);

    // QVector<QPointF>::iterator cit = polygon.begin();
    //qDebug("Polygon");
    //for(;cit < polygon.end(); ++cit)
    //{
    //    QPointF inScene = mpTargetNodeItem->mapToScene(*cit);
    //    qDebug("local coord: %.3f/%.3f", (cit)->x(), (cit)->y());
    //    qDebug("scene coord: %.3f/%.3f", inScene.x(), inScene.y());
    //}

    // Intersection with target
    QPointF p1 = item->mapToScene(polygon.first());
    QPointF p2;
    QPointF intersectionPoint;

    for(int i = 1; i < polygon.count(); ++i)
    {
        p2 = item->mapToScene(polygon.at(i));
        QLineF pLine(p1,p2);
        QLineF::IntersectType intersectType =
            pLine.intersect(line, &intersectionPoint);

        if(intersectType == QLineF::BoundedIntersection)
        {
            // intersection found
            // qDebug("Intersection found: at %.3f / %.3f",intersectionPoint.x(), intersectionPoint.y());
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
    qDebug("Hover ENTER event for %s", mpEdge->toString().c_str());
    mpGraphWidget->setSelectedEdge(mpEdge);
    mpGraphWidget->setEdgeSelected(true);
    QGraphicsItem::hoverEnterEvent(event);
}

void Simple::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(!mFocused)
    {
        mPen = mPenDefault;
    }
    mSelected = false;
    qDebug("Hover LEAVE event for %s", mpEdge->toString().c_str());
    mpGraphWidget->setEdgeSelected(false);
    QGraphicsItem::hoverLeaveEvent(event);
}

void Simple::grabFocus()
{
    mpGraphWidget->clearEdgeFocus();
    mPen = QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    update();
    mpGraphWidget->setEdgeFocused(true);
    mFocused = true;
    mpGraphWidget->setFocusedEdge(mpEdge);
}

void Simple::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
    mFocused ? releaseFocus() : grabFocus();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Simple::releaseFocus()
{
    mPen = mSelected ? QPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) : mPenDefault;
    update();
    mFocused = false;
    mpGraphWidget->setEdgeFocused(false);
}

} // end namespace edges
} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis

