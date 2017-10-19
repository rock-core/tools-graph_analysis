#include "EdgeItemBase.hpp"

#include "VertexItemBase.hpp"
#include "GraphWidget.hpp"
#include <base-logging/Logging.hpp>

#include <cmath>

#include <QDebug>

namespace graph_analysis {
namespace gui {

EdgeItemBase::EdgeItemBase()
    : QGraphicsItem(NULL)
    , mpEdge()
    , mpGraphWidget(NULL)
    , mpEdgePath(NULL)
    , mpArrowHead(NULL)
{}

EdgeItemBase::EdgeItemBase(GraphWidget* graphWidget,
                           const graph_analysis::Edge::Ptr& edge,
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

    graphWidget->registerEdgeItem(edge, this);

    mpEdgePath = new QGraphicsPathItem(QPainterPath(), this);
    mpArrowHead = new QGraphicsPolygonItem(this);
    mpArrowHead->setBrush(QBrush(Qt::black));
}

EdgeItemBase::~EdgeItemBase()
{
    if(mpGraphWidget && mpEdge)
    {
        mpGraphWidget->deregisterEdgeItem(mpEdge, this);
    }
    delete mpEdgePath;
    delete mpArrowHead;
}

int EdgeItemBase::type() const
{
    return EdgeItemBaseType;
}

void EdgeItemBase::adjustEdgePoints(QVector<QPointF> points)
{
    mPoints = points;
    if(mPoints.size() > 1)
    {
        adjustEdgePositioning();
    }
    else
    {
        // if the two Vertices which are connected by this Edge are located on
        // top of each other for example, it may be that no valid points could
        // be calculated. we shall not crash in this case.
        LOG_ERROR_S << "--- cannot adjst edge, know got " << mPoints.size()
                    << " points";
    }
}

void EdgeItemBase::drawStraightEdge()
{
    QPainterPath bezierPath;
    if(mPoints.size() >= 2)
    {
        QPointF start = mPoints.front();
        QPointF end = mPoints.back();

        bezierPath.moveTo(start);

        QPointF controlPoint0(start.x(), start.y());
        QPointF controlPoint1(end.x(), end.y());
        bezierPath.cubicTo(controlPoint0, controlPoint1, end);
        mpEdgePath->setPath(bezierPath);
    }
}

void EdgeItemBase::drawBezierEdge()
{
    QPainterPath bezierPath;
    if(mPoints.size() >= 2)
    {
        QPointF start = mPoints.front();
        QPointF end = mPoints.back();

        bezierPath.moveTo(start);

        float middleX = (start.x() + end.x())/2.0;
        QPointF controlPoint0(middleX, start.y());
        QPointF controlPoint1(middleX, end.y());

        bezierPath.cubicTo(controlPoint0, controlPoint1, end);
        mpEdgePath->setPath(bezierPath);
    }
}

void EdgeItemBase::drawArrowHead(size_t arrowSize)
{
    assert(mpEdgePath);

    QPointF referencePoint = mpEdgePath->path().pointAtPercent(0.9);
    QPointF lastPoint = mpEdgePath->path().pointAtPercent(1.0);

    QLineF lastSegment(referencePoint,
                       lastPoint);
    double angle = std::acos(lastSegment.dx() / lastSegment.length());
    // in case this is a very short edge we cannot infer how to actually draw
    // the arrow. in this case we'll fall back to not draw it.
    if(std::isnan(angle))
    {
        mpArrowHead->setPolygon(QPolygonF());
        return;
    }
    if(lastSegment.dy() >= 0)
    {
        angle = 2 * M_PI - angle;
    }

    QPointF destArrowP1 =
        lastPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                          cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = lastPoint +
                          QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                  cos(angle - M_PI + M_PI / 3) * arrowSize);
    mpArrowHead->setPolygon(QPolygonF() << lastPoint  << destArrowP1
                                        << destArrowP2);
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

QPainterPath EdgeItemBase::shape() const
{
    // reimplement "shape()" because the default implementation calls
    // boundingRect
    QPainterPath path;
    path = mpEdgePath->shape() + mpArrowHead->shape();
    return path;
}

} // end namespace gui
} // end namespace graph_analysis
