#include "GraphWidget.hpp"

#include <sstream>
#include <exception>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <base/Logging.hpp>
#include <base/Time.hpp>

#include <graph_analysis/io/GVGraph.hpp>

#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <graph_analysis/gui/WidgetManager.hpp>
#include <graph_analysis/gui/EdgeMimeData.hpp>

#include <graph_analysis/gui/BaseGraphView/AddVertexDialog.hpp>
#include <graph_analysis/gui/BaseGraphView/AddEdgeDialog.hpp>

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>


using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
    , mpScene(new QGraphicsScene(this))
    , mMaxNodeHeight(0)
    , mMaxNodeWidth (0)
    , mLayout("dot")
{
    mpScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(mpScene);
    setAcceptDrops(true);
}

GraphWidget::~GraphWidget()
{
    delete mpScene;
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
    updateStatus(status_msg, GraphWidgetManager::TIMEOUT);
}

void GraphWidget::updateStatus(const std::string& message, int timeout) const
{
    WidgetManager::getInstance()->getGraphWidgetManager()->updateStatus(
        QString(message.c_str()), timeout);
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

void GraphWidget::reset(bool keepData)
{
    WidgetManager::getInstance()->getGraphWidgetManager()->resetGraph(keepData);
}

void GraphWidget::resetLayoutingGraph()
{
}

void GraphWidget::update()
{
    updateView();

    QWidget::update();
}

void GraphWidget::updateView()
{
    updateLayoutView();
}

void GraphWidget::updateLayoutView()
{
    resetLayoutingGraph();

    // implemented by child-GraphWidgets. should create all QGraphicsItems of
    // the respective scene. needs to populate the layouting graph as needed.
    updateLayout();

    LOG_INFO_S << "restoring coordinates of " << mItemCoordinateMap.size()
               << " entries from cache";
    VertexItemCoordinateCache::iterator it = mItemCoordinateMap.begin();
    for(; it != mItemCoordinateMap.end(); it++)
    {
        VertexItemBase* item = mVertexItemMap[it->first];
        if(item)
        {
            // we have an item in the cache which is still in the scene. reuse
            // the old coordinate
            item->setPos(it->second);
        }
        else
        {
            // invalid entry in the coordinate cache. clean it.
            mItemCoordinateMap.erase(it);
        }
    }
}

// remove this implemntation?
void GraphWidget::shuffle()
{
    int diff = 600;
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
    reset(true /*keepData*/);

    update();
}

void GraphWidget::gvRender(const std::string& filename)
{
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

        case Qt::Key_R:
        {
            // CTRL+R deletes the graph (it first prompts again the user)
            WidgetManager::getInstance()->getGraphWidgetManager()->resetGraph();
            return;
        }

        case Qt::Key_E:
        case Qt::Key_S: {
            // CTRL+S (save) or CTRL+E (export graph) saves the graph to file
            WidgetManager::getInstance()->getGraphWidgetManager()->exportGraph();
            return;
        }

        case Qt::Key_O:
        case Qt::Key_I:
        {
            // CTRL+O (open) or CTRL+I (input graph)
            WidgetManager::getInstance()->getGraphWidgetManager()->importGraph();
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
                clearVisualization();
                update();
            }
        }

    }

    QGraphicsView::mouseDoubleClickEvent(event);
}

void GraphWidget::setFocusedElement(const GraphElement::Ptr &element)
{
    updateStatus("GraphElement: '" + element->getClassName() + " " +
                     element->getLabel() + "' (" + element->toString() + ")",
                 2500);
    mpFocusedElement = element;
}

void GraphWidget::clearFocus() { mpFocusedElement = GraphElement::Ptr(); }

void GraphWidget::registerEdgeItem(const graph_analysis::Edge::Ptr& e,
                                   EdgeItemBase* i)
{
    if(mEdgeItemMap.count(e))
    {
        LOG_ERROR_S << "re-registering existing edge item! " << e->toString();
    }
    mEdgeItemMap[e] = i;
}

void GraphWidget::registerVertexItem(const graph_analysis::Vertex::Ptr& v,
                                     VertexItemBase* i)
{
    if(mVertexItemMap.count(v))
    {
        LOG_ERROR_S << "re-registering existing vertex item! " << v->toString();
    }
    mVertexItemMap[v] = i;
}

void GraphWidget::deregisterEdgeItem(const graph_analysis::Edge::Ptr& e,
                                     EdgeItemBase* i)
{
    if(!mEdgeItemMap.count(e))
    {
        LOG_ERROR_S << "cannot deregister edge " << e->toString();
    }
    mEdgeItemMap.erase(e);
}

void GraphWidget::deregisterVertexItem(const graph_analysis::Vertex::Ptr& v,
                                       VertexItemBase* i)
{
    if(!mVertexItemMap.count(v))
    {
        LOG_ERROR_S << "cannot deregister vertex " << v->toString();
    }
    mVertexItemMap.erase(v);
}

EdgeItemBase*
GraphWidget::lookupEdgeItem(const graph_analysis::Edge::Ptr& e) const
{
    EdgeItemMap::const_iterator it = mEdgeItemMap.find(e);
    if(it == mEdgeItemMap.end())
    {
        /* throw std::runtime_error("cannot lookup egde item '" + e->toString() + */
        /*                          "'"); */
        return NULL;
    }
    return it->second;
}

VertexItemBase*
GraphWidget::lookupVertexItem(const graph_analysis::Vertex::Ptr& v) const
{
    VertexItemMap::const_iterator it = mVertexItemMap.find(v);
    if(it == mVertexItemMap.end())
    {
        /* throw std::runtime_error("cannot lookup vertex item '" + v->toString() + */
        /*                          "'"); */
        return NULL;
    }
    return it->second;
}

void GraphWidget::cacheVertexItemPosition(const graph_analysis::Vertex::Ptr v,
                                          QPointF p)
{
    mItemCoordinateMap[v] = p;
}

} // end namespace gui
} // end namespace graph_analysis
