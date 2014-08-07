#include "Simple.hpp"
#include <omviz/graphwidget/graphitem/edges/EdgeLabel.hpp>
#include <omviz/graphwidget/NodeItem.hpp>

namespace omviz {
namespace graphitem {
namespace edges {

Simple::Simple(NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge)
    : EdgeItem(sourceNode, targetNode, edge)
{
    mLabel = new EdgeLabel(edge->toString(), this);
}

void Simple::adjust()
{
    if (!mpSourceNodeItem || !mpTargetNodeItem)
        return;

    QLineF line(mapFromItem(mpSourceNodeItem, 0, 0), mapFromItem(mpTargetNodeItem, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        mSourcePoint = line.p1() + edgeOffset;
        mTargetPoint = line.p2() - edgeOffset;
    } else {
        mSourcePoint = mTargetPoint = line.p1();
    }

    QPointF centerPos((mTargetPoint.x() - mSourcePoint.x())/2.0, (mTargetPoint.y() - mSourcePoint.y())/2.0);
    mLabel->setPos(centerPos);
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

void Simple::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!mpSourceNodeItem || !mpTargetNodeItem)
        return;

    QLineF line(mSourcePoint, mTargetPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF sourceArrowP1 = mSourcePoint + QPointF(sin(angle + Pi / 3) * mArrowSize,
                                                  cos(angle + Pi / 3) * mArrowSize);
    QPointF sourceArrowP2 = mSourcePoint + QPointF(sin(angle + Pi - Pi / 3) * mArrowSize,
                                                  cos(angle + Pi - Pi / 3) * mArrowSize);
    QPointF destArrowP1 = mTargetPoint + QPointF(sin(angle - Pi / 3) * mArrowSize,
                                              cos(angle - Pi / 3) * mArrowSize);
    QPointF destArrowP2 = mTargetPoint + QPointF(sin(angle - Pi + Pi / 3) * mArrowSize,
                                              cos(angle - Pi + Pi / 3) * mArrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}

} // end namespace edges
} // end namespace graphitem
} // end namespace omviz

