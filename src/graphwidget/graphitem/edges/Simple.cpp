#include "Simple.hpp"
#include <omviz/graphwidget/graphitem/edges/EdgeLabel.hpp>
#include <omviz/graphwidget/NodeItem.hpp>

namespace omviz {
namespace graphitem {
namespace edges {

Simple::Simple(NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge)
    : EdgeItem(sourceNode, targetNode, edge)
{
    mpLabel = new EdgeLabel(edge->toString(), this);
    mPen = QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void Simple::adjust()
{
    if (!mpSourceNodeItem || !mpTargetNodeItem)
        return;

    QLineF line(mapFromItem(mpSourceNodeItem, 0, 0), mapFromItem(mpTargetNodeItem, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    {
        mTargetPoint = mpTargetNodeItem->pos();
        qreal width = mpTargetNodeItem->boundingRect().width()/2.0;
        qreal height = mpTargetNodeItem->boundingRect().height()/2.0;
        mTargetPoint = QPointF( mTargetPoint.x() + width, mTargetPoint.y() + height);
    }

    {
        mSourcePoint = mpSourceNodeItem->pos();
        qreal width = mpSourceNodeItem->boundingRect().width()/2.0;
        qreal height = mpSourceNodeItem->boundingRect().height()/2.0;
        mSourcePoint = QPointF( mSourcePoint.x() + width, mSourcePoint.y() + height);
    }

    QPointF centerPos((mTargetPoint.x() - mSourcePoint.x())/2.0, (mTargetPoint.y() - mSourcePoint.y())/2.0);
    mpLabel->setPos(centerPos);
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

    QLineF line(mSourcePoint, mTargetPoint);
    QPointF intersectionPointWithSource = getIntersectionPoint(mpSourceNodeItem, line);
    QPointF intersectionPointWithTarget = getIntersectionPoint(mpTargetNodeItem, line);

    QLineF boundedLine(intersectionPointWithSource, intersectionPointWithTarget);

    // Draw the line itself
    painter->setPen(mPen);
    painter->drawLine(boundedLine);

    // Draw the arrows
    double angle = ::acos(boundedLine.dx() / boundedLine.length());
    if (boundedLine.dy() >= 0)
        angle = TwoPi - angle;

    QPointF destArrowP1 = intersectionPointWithTarget + QPointF(sin(angle - Pi / 3) * mArrowSize,
                                              cos(angle - Pi / 3) * mArrowSize);
    QPointF destArrowP2 = intersectionPointWithTarget + QPointF(sin(angle - Pi + Pi / 3) * mArrowSize,
                                              cos(angle - Pi + Pi / 3) * mArrowSize);

    painter->setBrush(mPen.brush());
    painter->drawPolygon(QPolygonF() << boundedLine.p2() << destArrowP1 << destArrowP2);
}

QPointF Simple::getIntersectionPoint(NodeItem* item, const QLineF& line)
{
    QPolygonF polygon = item->boundingRect();

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
        p2 = item->mapToParent(polygon.at(i));
        QLineF pLine(p1,p2);
        QLineF::IntersectType intersectType = 
            pLine.intersect(line, &intersectionPoint);

        if( intersectType == QLineF::BoundedIntersection)
        {
            // intersection found
            qDebug("Intersection found: at %.3f / %.3f",intersectionPoint.x(), intersectionPoint.y());
            break;
        } else {
            // no intersection fonuu
            qDebug("No intersection found");
            p1 = p2;
        }
    }
    return intersectionPoint;
}

} // end namespace edges
} // end namespace graphitem
} // end namespace omviz

