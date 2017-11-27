#include "GraphWidget.hpp"

#include <base-logging/Logging.hpp>

#include "../VertexTypeManager.hpp"
#include "../EdgeTypeManager.hpp"

#include "EdgeMimeData.hpp"
#include "BaseGraphView/AddVertexDialog.hpp"
#include "BaseGraphView/AddEdgeDialog.hpp"
#include "../io/GVGraph.hpp"
#include "Player.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>

#include <math.h>
#include <iostream>

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

GraphWidget::GraphWidget(const graph_analysis::BaseGraph::Ptr& graph, QWidget* parent)
    : QGraphicsView(parent)
    , mpScene(new QGraphicsScene(this))
    , mpGraph(graph)
{
    mpScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(mpScene);
    setAcceptDrops(true);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                   QPainter::SmoothPixmapTransform);
}

GraphWidget::~GraphWidget()
{
    delete mpScene;
}

graph_analysis::BaseGraph::Ptr GraphWidget::graph() const
{
    return mpGraph;
}

void GraphWidget::setGraph(const graph_analysis::BaseGraph::Ptr& graph)
{
    mpGraph = graph;
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2.0, -event->delta() / 240.0));
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform()
                       .scale(scaleFactor, scaleFactor)
                       .mapRect(QRectF(0, 0, 1, 1))
                       .width();
    if(factor < 0.07 || factor > 100)
    {
        return;
    }
    scale(scaleFactor, scaleFactor);
    std::string status_msg = scaleFactor > 1. ? "Zoomed-in" : "Zoomed-out";
    updateStatus(status_msg, 1000);
}

void GraphWidget::updateStatus(const std::string& message, int timeout)
{
    emit currentStatus(QString(message.c_str()), timeout);
}

void GraphWidget::play(const PlayerConfiguration& configuration)
{
    Player player(this);
    player.start(configuration);
}

void GraphWidget::clearVisualization()
{
    // calling "clear()" on the scene correctly disposes of all the
    // objetcs allocated. looks like so, the dtors are called... puh!
    scene()->clear();
    // afterwards we can clear the "GraphElement to Item" caches.
    mEdgeItemMap.clear();
    mVertexItemMap.clear();
}

void GraphWidget::update()
{
    // trigger a recreation of the vertex and edge items
    applyCachedLayout();
    QWidget::update();
}

void GraphWidget::setVertexVisible(int id, bool visible)
{
    Vertex::Ptr v = mpGraph->getVertex(id);
    VertexItemMap::iterator it = mVertexItemMap.find(v);
    if(it != mVertexItemMap.end())
    {
        it->second->setVisible(visible);
    }
}

void GraphWidget::setEdgeVisible(int id, bool visible)
{
    Edge::Ptr v = mpGraph->getEdge(id);
    EdgeItemMap::iterator it = mEdgeItemMap.find(v);
    if(it != mEdgeItemMap.end())
    {
        it->second->setVisible(visible);
    }
}

GraphWidget::VertexItemCoordinateCache GraphWidget::getCurrentLayout() const
{
    VertexItemCoordinateCache vertexItemCoordinates;
    VertexItemMap::const_iterator cit = mVertexItemMap.begin();
    for(; cit != mVertexItemMap.end(); ++cit)
    {
        Vertex::Ptr vertex = cit->first;
        VertexItemBase* item = cit->second;
        vertexItemCoordinates[vertex] = item->pos();
    }
    return vertexItemCoordinates;
}

void GraphWidget::applyCachedLayout()
{
    applyLayout(mItemCoordinateCache);
}

void GraphWidget::cacheCurrentLayout()
{
    mItemCoordinateCache = getCurrentLayout();
}

GraphWidget::VertexItemCoordinateCache GraphWidget::applyLayout(const VertexItemCoordinateCache& _coordinates)
{
    VertexItemCoordinateCache coordinates = _coordinates;
    // implemented by child-GraphWidgets. should create all QGraphicsItems of
    // the respective scene. needs to populate the layouting graph as needed.
    populateCanvas();

    if(coordinates.empty())
    {
        LOG_WARN_S << "cannot apply layout: no item coordinates provided";
        return coordinates;
    }

    LOG_INFO_S << "restoring coordinates of " << coordinates.size()
               << " entries from cache";
    VertexItemCoordinateCache::iterator it = coordinates.begin();
    for(; it != coordinates.end(); ++it)
    {
        VertexItemBase* item = mVertexItemMap[it->first];
        if(item)
        {
            // we have an item in the cache which is still in the scene.
            // use the cached coordinates
            item->setPos(it->second);
        }
        else
        {
            // invalid entry in the coordinate cache. clean it.
            LOG_DEBUG_S << "deleting invalid entry '" << it->first->toString()
                        << "' from cache";
            coordinates.erase(it);
        }
    }
    return coordinates;
}

// remove this implementation?
void GraphWidget::shuffle()
{
    int diff = 25 *mVertexItemMap.size();
    foreach(QGraphicsItem *item, scene()->items())
    {
        if(dynamic_cast<VertexItemBase *>(item)) {
            item->setPos(-diff/2 + qrand() % diff, -diff/2 + qrand() % diff);
        }
    }
    updateStatus(
        "Shuffelled all nodes representing a 'Vertex' of basic GraphWidget");
}

void GraphWidget::refresh(bool all)
{
    LOG_DEBUG_S << "Refresh widget: " << getClassName().toStdString() << " keep all data";
    update();
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    // check some key combinations which will be accepting while holding the
    // 'ctrl' key:
    if(event->modifiers() & Qt::ControlModifier)
    {
        switch(event->key())
        {
        case Qt::Key_Q:
        case Qt::Key_W:
        {
            // CTRL+Q and CTRL+W terminate the application
            QApplication::quit();
            // well... return? hrhr...
            return;
        }

        case Qt::Key_L:
        {
            // re-layouting/re-displaying
            updateVisualization();
            return;
        }

        case Qt::Key_R:
        {
            // "random" layout
            shuffle();
            return;
        }

        case Qt::Key_E:
        case Qt::Key_S: {
            // CTRL+S (save) or CTRL+E (export graph) saves the graph to file
            /* WidgetManager::getInstance()->getGraphWidgetManager()->exportGraph(); */
            return;
        }

        case Qt::Key_O:
        case Qt::Key_I:
        {
            // CTRL+O (open) or CTRL+I (input graph)
            /* WidgetManager::getInstance()->getGraphWidgetManager()->importGraph(); */
            return;
        }
        }
    }
    // if we did not act, we shall call the handler of the base-class to
    // propagate the event.
    QGraphicsView::keyPressEvent(event);
}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
    // enable panning by pressing+dragging the left mouse button if there is
    // _no_ item under the cursor right now.
    if ((event->button() == Qt::LeftButton) && (!itemAt(event->pos()))) {
        setDragMode(ScrollHandDrag);
        QGraphicsView::mousePressEvent(event);
        return;
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // always try to reset drag mode, just to be sure
    if (dragMode() != QGraphicsView::NoDrag) {
        setDragMode(NoDrag);
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void GraphWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        // no item where the user clicked
        if(!itemAt(event->pos()))
        {
            AddVertexDialog dialog;
            dialog.exec();
            if(dialog.result() == QDialog::Accepted)
            {
                graph()->addVertex(
                    VertexTypeManager::getInstance()->createVertex(
                        dialog.getClassname().toStdString(),
                        dialog.getLabel().toStdString()));
            }
        }

    }

    QGraphicsView::mouseDoubleClickEvent(event);
}

void GraphWidget::updateVisualization()
{
    clearVisualization();
    update();
}

void GraphWidget::setFocusedElement(const GraphElement::Ptr &element)
{
    updateStatus("GraphElement: '" + element->getClassName() + " " +
                     element->getLabel() + "' (" + element->toString() + ")",
                 2500);
    mpFocusedElement = element;
}

void GraphWidget::clearFocus() { updateStatus("", 2500);mpFocusedElement = GraphElement::Ptr(); }

void GraphWidget::registerEdgeItem(const graph_analysis::Edge::Ptr& e,
                                   EdgeItemBase* i)
{
    if(mEdgeItemMap.count(e) > 0)
    {
        LOG_INFO_S << "re-registering existing edge item! " << e->toString();
    }
    mEdgeItemMap[e] = i;
}

void GraphWidget::registerVertexItem(const graph_analysis::Vertex::Ptr& v,
                                     VertexItemBase* i)
{
    if(mVertexItemMap.count(v) > 0)
    {
        LOG_INFO_S << "re-registering existing vertex item! " << v->toString();
    }
    mVertexItemMap[v] = i;
}

void GraphWidget::deregisterEdgeItem(const graph_analysis::Edge::Ptr& e,
                                     EdgeItemBase* i)
{
    if(mEdgeItemMap.count(e) > 0)
    {
        mEdgeItemMap.erase(e);
    } else {
        LOG_INFO_S << "cannot deregister edge " << e->toString() << " -- edge has never been registered";
    }
}

void GraphWidget::deregisterVertexItem(const graph_analysis::Vertex::Ptr& v,
                                       VertexItemBase* i)
{
    if(mVertexItemMap.count(v) > 0)
    {
        mVertexItemMap.erase(v);
    } else {
        LOG_INFO_S << "cannot deregister vertex " << v->toString() << " -- vertex has never been registered";
    }
}

QVector<QPointF> GraphWidget::getEdgePoints(VertexItemBase* firstItem,
                                            VertexItemBase* secondItem) const
{
    QVector<QPointF> retval;
    QLineF directConnection(firstItem->getConnector(),
                            secondItem->getConnector());
    QPolygonF b1 = firstItem->mapToScene(firstItem->boundingRect());
    if(b1.containsPoint(directConnection.p1(), Qt::OddEvenFill))
    {
        for(int i = 1; i < b1.size(); i++)
        {
            QLineF l(b1.at(i - 1), b1.at(i));
            QPointF p;
            enum QLineF::IntersectType type = directConnection.intersect(l, &p);
            if(type == QLineF::BoundedIntersection)
            {
                retval.push_back(p);
                // only use the first intersection
                break;
            }
        }
    }
    // fall back to use just the connector, in case we did not get anything.
    if (retval.size() < 1)
    {
        retval.push_back(firstItem->getConnector());
    }
    QPolygonF b2 = secondItem->mapToScene(secondItem->boundingRect());
    if(b2.containsPoint(directConnection.p2(), Qt::OddEvenFill))
    {
        for(int i = 1; i < b2.size(); i++)
        {
            QLineF l(b2.at(i - 1), b2.at(i));
            QPointF p;
            enum QLineF::IntersectType type = directConnection.intersect(l, &p);
            if(type == QLineF::BoundedIntersection)
            {
                retval.push_back(p);
                // only use the first intersection
                break;
            }
        }
    }
    if(retval.size() < 2)
    {
        retval.push_back(secondItem->getConnector());
    }
    return retval;
}

// this is intended for vertices which have edges connected to them
void GraphWidget::vertexPositionHasChanged(VertexItemBase* item)
{
    updateEdgePositions(item);

    // also cache the position so that we can reload it in case we have to
    // create the current layout after a reset.
    if(item->flags() & QGraphicsItem::ItemIsMovable)
    {
        cacheVertexItemPosition(item->getVertex(), item->pos());
    }
}

void GraphWidget::updateEdgePositions(VertexItemBase* item)
{
    // checkout all edges of this vertex, and search for these which are
    // registered as having an actual item associated with them.
    EdgeIterator::Ptr edgeIt = graph()->getEdgeIterator(item->getVertex());
    while(edgeIt->next())
    {
        EdgeItemMap::iterator edgeItemIt = mEdgeItemMap.find(edgeIt->current());

        // if this "Edge" has an actual graphical representation on the canvas:
        if(edgeItemIt != mEdgeItemMap.end())
        {
            VertexItemBase* targetItem =
                mVertexItemMap[edgeItemIt->first->getTargetVertex()];
            VertexItemBase* sourceItem =
                mVertexItemMap[edgeItemIt->first->getSourceVertex()];

            QVector<QPointF> points = getEdgePoints(sourceItem, targetItem);
            EdgeItemBase* edgeItem = edgeItemIt->second;
            edgeItem->adjustEdgePoints(points);
        }
    }
}

void GraphWidget::cacheVertexItemPosition(const graph_analysis::Vertex::Ptr v,
                                          QPointF p)
{
    mItemCoordinateCache[v] = p;
    // this map should only contain entries for movable items on the canvas.
    // subitems, which are not individually movable, should not cache in this
    // map.
    if(!(mVertexItemMap[v]->flags() & QGraphicsItem::ItemIsMovable))
    {
        LOG_ERROR_S << "unmovable item in coordinate map: '" << v->toString()
                    << "'";
    }
}

} // end namespace gui
} // end namespace graph_analysis
