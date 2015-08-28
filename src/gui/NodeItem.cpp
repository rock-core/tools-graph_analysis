#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "GraphWidget.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include <base/Logging.hpp>
#include <boost/assign/list_of.hpp>

namespace graph_analysis {
namespace gui {

NodeItem::NodeItem(GraphWidget *graphWidget, graph_analysis::Vertex::Ptr vertex)
    : mpVertex(vertex)
    , mpGraphWidget(graphWidget)
{
    setFlag(ItemIsMovable, false);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setToolTip(QString(vertex->getClassName().c_str()));
}

void NodeItem::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this)
    {
        mNewPos = pos();
        return;
    }
    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items())
    {
        NodeItem* node = qgraphicsitem_cast<NodeItem* >(item);
        if (!node)
        {
            continue;
        }
        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0)
        {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }
    // Now subtract all forces pulling items together
    GraphWidget::EdgeItemMap::iterator it = mpGraphWidget->edgeItemMap().begin();
    double weight = (mpGraphWidget->edgeItemMap().size() + 1) * 10;
    for(; it != mpGraphWidget->edgeItemMap().end(); ++it)
    {
        EdgeItem* edge = it->second;
        QPointF vec;
        if (edge->sourceNodeItem() == this)
        {
            vec = mapToItem(edge->targetNodeItem(), 0, 0);
        }
        else
        {
            vec = mapToItem(edge->sourceNodeItem(), 0, 0);
        }
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }
    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
    {
        xvel = yvel = 0;
    }
    QRectF sceneRect = scene()->sceneRect();
    mNewPos = pos() + QPointF(xvel, yvel);
    mNewPos.setX(qMin(qMax(mNewPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    mNewPos.setY(qMin(qMax(mNewPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}

bool NodeItem::advance()
{
    if (mNewPos != pos())
    {
        setPos(mNewPos);
        return true;
    }
    return false;
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch (change)
    {
        case ItemPositionHasChanged:
        {
            GraphWidget::EdgeItemMap::iterator it = mpGraphWidget->edgeItemMap().begin();
            for(; it != mpGraphWidget->edgeItemMap().end(); ++it)
            {
                EdgeItem* edge = it->second;
                if(edge)
                {
                    edge->adjust();
                }
            }
            mpGraphWidget->itemMoved();
            break;
        }
        default:
            break;
    };
    return QGraphicsItem::itemChange(change, value);
}

QPointF NodeItem::getCenterPosition() const
{
    QRectF rect = boundingRect();
    QPointF position = pos();

    qreal width = rect.width()/2.0;
    qreal height = rect.height()/2.0;
    return QPointF( position.x() + width, position.y() + height);
}

std::string NodeItem::getId() const
{
    char buffer[512];
    snprintf(buffer,512, "%p-%s",this, mpVertex->toString().c_str());
    return std::string(buffer);
}

} // end namespace gui
} // end namespace graph_analysis
