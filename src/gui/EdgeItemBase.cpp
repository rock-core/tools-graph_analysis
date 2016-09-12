#include "EdgeItemBase.hpp"

#include <graph_analysis/gui/VertexItemBase.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <base-logging/Logging.hpp>

#include <cmath>

#include <QDebug>

namespace graph_analysis {
namespace gui {

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

} // end namespace gui
} // end namespace graph_analysis
