#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "GraphWidget.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include <base/Logging.hpp>
#include <boost/assign/list_of.hpp>
#include <graph_analysis/gui/items/Feature.hpp>

namespace graph_analysis {
namespace gui {

NodeItem::NodeItem(GraphWidget *graphWidget, graph_analysis::Vertex::Ptr vertex)
    : QGraphicsItemGroup()
    , mpVertex(vertex)
    , mpGraphWidget(graphWidget)
{
    setFlag(ItemIsMovable, false);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setToolTip(QString(vertex->getClassName().c_str()));
    setAcceptDrops(true);
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
    GraphWidget::EdgeItemMap::const_iterator it = mpGraphWidget->edgeItemMap().begin();
    double weight = (mpGraphWidget->edgeItemMap().size() + 1) * 10;
    for(; it != mpGraphWidget->edgeItemMap().end(); ++it)
    {
        EdgeItem* edge = it->second;
        QPointF vec;
        if (edge->sourceItem() == this)
        {
            vec = mapToItem(edge->targetItem(), 0, 0);
        }
        else
        {
            vec = mapToItem(edge->sourceItem(), 0, 0);
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
            GraphWidget::EdgeItemMap::const_iterator it = mpGraphWidget->edgeItemMap().begin();
            for(; it != mpGraphWidget->edgeItemMap().end(); ++it)
            {
                EdgeItem* edge = it->second;
                if(edge)
                {
                    edge->adjust();
                }
            }
            break;
        }
        default:
            break;
    };
    return QGraphicsItem::itemChange(change, value);
}

std::string NodeItem::getId() const
{
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "%p-%s", (void *)this,
             mpVertex->toString().c_str());
    return std::string(buffer);
}

void NodeItem::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    LOG_INFO_S << "Drop event for node item of '" << mpVertex->toString() << "' ";
    if(mimeData && mimeData->hasText())
    {
        LOG_INFO_S << "    drag from source graph element with id: " << mimeData->text().toStdString();
    }
}

const items::Feature* NodeItem::getPointedAtFeature() const
{
    std::vector<items::Feature*>::const_iterator cit = mFeatures.begin();

    std::vector<const items::Feature*> pointedAtFeatures;

    for(; cit != mFeatures.end(); ++cit)
    {
        const items::Feature* feature = *cit;
        if(feature->isUnderMouse())
        {
            pointedAtFeatures.push_back(feature);
        }
    }
    if(pointedAtFeatures.empty())
    {
        return NULL;
    } else if(pointedAtFeatures.size() > 1)
    {
        LOG_WARN_S << "Mouse pointer over multiple features";
        return pointedAtFeatures.at(0);
    } else {
        return pointedAtFeatures.at(0);
    }
}

Vertex::Ptr NodeItem::getPointedAtVertex() const
{
    const items::Feature* feature = getPointedAtFeature();
    if(feature)
    {
        return feature->getVertex();
    } else {
        return mpVertex;
    }
}

NodeItem::id_t NodeItem::addFeature(items::Feature* feature)
{
    std::vector<items::Feature*>::const_iterator cit = std::find(mFeatures.begin(), mFeatures.end(), feature);
    if(cit == mFeatures.end())
    {
        mFeatures.push_back(feature);
        return mFeatures.size() - 1;
    } else {
        throw std::invalid_argument("graph_analysis::gui::NodeItem::addFeature: feature already added");
    }
}

void NodeItem::removeFeature(NodeItem::id_t pos)
{
    if(mFeatures.size() > pos)
    {
        // Remove from base graph
        mpGraphWidget->graph()->removeVertex( mFeatures[pos]->getVertex() );
        // Remove from node item
        mFeatures.erase( mFeatures.begin() + pos );
    } else {
        throw std::invalid_argument("graph_analysis::gui::NodeItem::removeFeature: index out of bounds");
    }
}

void NodeItem::removeFeatures()
{
    while(!mFeatures.empty())
    {
        removeFeature(0);
    }
}

items::Feature* NodeItem::getFeature(NodeItem::id_t pos) const
{
    return mFeatures.at(pos);
}

void NodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    // Set the underlaying vertex as focused element
    mpGraphWidget->setFocusedElement(mpVertex);
}

} // end namespace gui
} // end namespace graph_analysis
