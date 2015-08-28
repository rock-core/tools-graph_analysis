#include "EdgeItem.hpp"
#include "NodeItem.hpp"

#include <math.h>
#include <QPainter>
#include <base/Logging.hpp>

const double graph_analysis::gui::EdgeItem::Pi = 3.14159265358979323846264338327950288419717;
double graph_analysis::gui::EdgeItem::TwoPi = 2.0 * Pi;

namespace graph_analysis {
namespace gui {

EdgeItem::EdgeItem(GraphWidget* graphWidget, NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge)
    : mpGraphWidget(graphWidget)
    , mpSourceNodeItem(sourceNode)
    , mpTargetNodeItem(targetNode)
    , mpEdge(edge)
    , mArrowSize(10)
{
    setAcceptedMouseButtons(0);
    adjust();
}

NodeItem* EdgeItem::sourceNodeItem() const
{
    return mpSourceNodeItem;
}

NodeItem* EdgeItem::targetNodeItem() const
{
    return mpTargetNodeItem;
}

void EdgeItem::adjust()
{
    if (!mpSourceNodeItem || !mpTargetNodeItem)
    {
        // skipping when one of the endpoints is invalid
        return;
    }

    QLineF line(mapFromItem(mpSourceNodeItem, 0, 0), mapFromItem(mpTargetNodeItem, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.))
    {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        mSourcePoint = line.p1() + edgeOffset;
        mTargetPoint = line.p2() - edgeOffset;
    } else {
        mSourcePoint = mTargetPoint = line.p1();
    }
}

QRectF EdgeItem::boundingRect() const
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

void EdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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

QPainterPath EdgeItem::shape() const
{
    return mPainterPath;
    return ::QGraphicsItem::shape();
}

} // end namespace gui
} // end namespace graph_analysis
