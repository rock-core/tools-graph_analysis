#include "EdgeItem.hpp"
#include "NodeItem.hpp"

#include <math.h>
#include <QPainter>
#include <base-logging/Logging.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis {
namespace gui {

EdgeItem::EdgeItem(GraphWidget* graphWidget, QGraphicsItem* source, QGraphicsItem* target, graph_analysis::Edge::Ptr edge)
    : mpGraphWidget(graphWidget)
    , mpSourceItem(source)
    , mpTargetItem(target)
    , mpEdge(edge)
    , mArrowSize(10)
{
    if(!edge)
    {
        throw std::invalid_argument("graph_analysis::gui::EdgeItem: construction failed -- underlying edge is NULL");
    }
    if(!source)
    {
        throw std::invalid_argument("graph_analysis::gui::EdgeItem: construction failed for '" + edge->toString() + "' -- source graphics item is NULL");
    } else if(!target) {
        throw std::invalid_argument("graph_analysis::gui::EdgeItem: construction failed for '" + edge->toString() + "' -- target graphics item is NULL");
    }
    setAcceptedMouseButtons(Qt::NoButton);
    adjust();
}

void EdgeItem::adjust()
{
    if (!mpSourceItem || !mpTargetItem)
    {
        // skipping when one of the endpoints is invalid
        return;
    }

    QLineF line(mapFromItem(mpSourceItem, 0, 0), mapFromItem(mpTargetItem, 0, 0));
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
    if (!mpSourceItem || !mpTargetItem)
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
    LOG_DEBUG_S << "PAINTING EDGE ITEM";
    if (!mpSourceItem || !mpTargetItem)
    {
        LOG_DEBUG_S << "PAINTING EDGE ITEM: no source or target item";
        return;
    }

    QLineF line(mSourcePoint, mTargetPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = 2*M_PI - angle;

    QPointF sourceArrowP1 = mSourcePoint + QPointF(sin(angle + M_PI / 3) * mArrowSize,
                                                  cos(angle + M_PI / 3) * mArrowSize);
    QPointF sourceArrowP2 = mSourcePoint + QPointF(sin(angle + M_PI - M_PI / 3) * mArrowSize,
                                                  cos(angle + M_PI - M_PI / 3) * mArrowSize);
    QPointF destArrowP1 = mTargetPoint + QPointF(sin(angle - M_PI / 3) * mArrowSize,
                                              cos(angle - M_PI / 3) * mArrowSize);
    QPointF destArrowP2 = mTargetPoint + QPointF(sin(angle - M_PI + M_PI / 3) * mArrowSize,
                                              cos(angle - M_PI + M_PI / 3) * mArrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}

void EdgeItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    // Set the current edge as focused element
    mpGraphWidget->setFocusedElement(mpEdge);
}

void EdgeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    // Set the current edge as focused element
    mpGraphWidget->clearFocus();
}

} // end namespace gui
} // end namespace graph_analysis
