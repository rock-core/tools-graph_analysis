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
    , mpGVGraph(NULL)
    , mMaxNodeHeight(0)
    , mMaxNodeWidth (0)
    , mScaleFactor(2.2)
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

    // does this leak memory like a bucket...
    e_map.clear();
    v_map.clear();
    // ...or does calling "clear()" on the scene correctly disposes of all the
    // objetcs allocated? looks like to, the dtors are called... puh!
    scene()->clear();

    if(mpGVGraph)
    {
        mpGVGraph->clearEdges();
        mpGVGraph->clearNodes();
    }

}

void GraphWidget::reset(bool keepData)
{
    WidgetManager::getInstance()->getGraphWidgetManager()->resetGraph(keepData);
}

void GraphWidget::resetLayoutingGraph()
{
    if(mpGVGraph)
    {
        delete mpGVGraph;
    }

    mpLayoutingGraph = BaseGraph::getInstance();
    mpGVGraph = new io::GVGraph(mpLayoutingGraph, "LayoutingGraph");
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

            qDebug() << "restoring from cache. have"<<coordindate_map.size()<<"entries";
    VertexItemCoordinateCache::iterator it = coordindate_map.begin();
    for(; it != coordindate_map.end(); it++)
    {
        VertexItemBase* item = v_map[it->first];
        if(item)
        {
            // we have an item in the cache which is still in the scene. reuse
            // the old coordinate
            qDebug() << "setting pos from" << item->pos() << "to" << it->second;
            item->setPos(it->second);
        }
        else
        {
            qDebug() << "vertex"<<it->first->toString().c_str()<<"not in cache";
            // invalid entry in the coordinate cache. clean it.
            coordindate_map.erase(it);
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
    try
    {
        mpGVGraph->renderToFile(filename, mLayout.toStdString());
    }
    catch(std::runtime_error e)
    {
        LOG_ERROR_S << "graph_analysis::gui::GraphWidgetManager::toDotFile: export via graphviz failed: " << e.what();
        QMessageBox::critical(this, tr("Graph export via GraphViz failed"), e.what());
        updateStatus("Dot Graph export failed: " + std::string(e.what()));
    }
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
    LOG_INFO_S << "GraphWidget: pressEvent";

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
        else
        {
            Vertex::Ptr sourceVertex;
            Vertex::Ptr targetVertex;
            // obtain the vertex of the item the user clicked on
            QList<QGraphicsItem*> clickedItems = items(event->pos());
            for(int i = 0; i < clickedItems.size(); i++)
            {
                VertexItemBase* sourceItem =
                    dynamic_cast<VertexItemBase*>(clickedItems.at(i));
                if(sourceItem)
                {
                    sourceVertex = sourceItem->getVertex();
                    break;
                }
            }
            if(!sourceVertex)
            {
                LOG_ERROR_S << "could not find a source vertex for dragEvent";
                return;
            }

            QDrag* drag = new QDrag(this);
            // stores reference to the two vertices, so that the receiving side
            // can do error-checking and store its vertex as target on success.
            EdgeMimeData* mimeData = new EdgeMimeData(sourceVertex, targetVertex);

            drag->setMimeData(mimeData);

            // when this returns, the user finished its drag-operation
            Qt::DropAction dropAction = drag->exec();

            if(dropAction == Qt::MoveAction)
            {
                // check that the targetVertex got updated
                if(!targetVertex)
                {
                    LOG_ERROR_S
                        << "could not find a target vertex after dropEvent";
                    return;
                }
                AddEdgeDialog dialog;
                dialog.exec();
                if(dialog.result() == QDialog::Accepted)
                {
                    Edge::Ptr edge = EdgeTypeManager::getInstance()->createEdge(
                        dialog.getClassname().toStdString(), sourceVertex,
                        targetVertex, dialog.getLabel().toStdString());
                    graph()->addEdge(edge);
                    clearVisualization();
                    update();
                }
            }
        }
    }
}

void GraphWidget::setFocusedElement(const GraphElement::Ptr &element)
{
    updateStatus("GraphElement: '" + element->getClassName() + " " +
                     element->getLabel() + "' (" + element->toString() + ")",
                 2500);
    mpFocusedElement = element;
}

void GraphWidget::clearFocus() { mpFocusedElement = GraphElement::Ptr(); }

} // end namespace gui
} // end namespace graph_analysis
