#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "FilterItem.hpp"
#include "LayerViewWidget.hpp"
#include "IconManager.hpp"
#include "GraphManager.hpp"
#include "WidgetManager.hpp"
#include "AddNodeDialog.hpp"
#include "FilterManager.hpp"
#include "PropertyDialog.hpp"
#include "NodeTypeManager.hpp"
#include "EdgeTypeManager.hpp"
#include "ActionCommander.hpp"

#include <set>
#include <math.h>
#include <string>
#include <sstream>
#include <QDir>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSignalMapper>
#include <boost/regex.hpp>
#include <base/Logging.hpp>


#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/BaseVertex.hpp>
#include <graph_analysis/PortVertex.hpp>
#include <graph_analysis/InputPortVertex.hpp>
#include <graph_analysis/OutputPortVertex.hpp>
#include <graph_analysis/ClusterVertex.hpp>
#include <graph_analysis/PropertyVertex.hpp>
#include <graph_analysis/OperationVertex.hpp>

#include "layeritem/Resource.hpp"
#include "layeritem/edges/Simple.hpp"
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>
#include <graph_analysis/filters/RegexFilters.hpp>

#include <exception>
#include <boost/foreach.hpp>
#include <base/Time.hpp>

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

LayerViewWidget::LayerViewWidget(graph_analysis::BaseGraph::Ptr graph, QWidget *parent)
    : GraphWidget(graph, parent)
    , mFeatureLayerToggle(true)
    , mClusterLayerToggle(true)
{
    // Add seed for force layout
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    setCacheMode(CacheBackground);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Graphview"));
    // Setting up filtering
    mGraphView.setVertexFilter(mpVertexFilter);
    mGraphView.setEdgeFilter(mpEdgeFilter);
    // End of setting up filters

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(showContextMenu(const QPoint &)));

    reset(true);
    updateFromGraph();
}

LayerViewWidget::~LayerViewWidget()
{
//    destroy();
}

void LayerViewWidget::showContextMenu(const QPoint& pos)
{
    ActionCommander comm(WidgetManager::getInstance()->getComponentEditorWidget());
    QMenu contextMenu(tr("Context menu"), this);

    QAction *actionRefresh = comm.addAction("Refresh", SLOT(refresh()), *(IconManager::getInstance()->getIcon("refresh")), this);
    QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffle()), *(IconManager::getInstance()->getIcon("shuffle")), this);
    QAction *actionImport  = comm.addAction("Import", SLOT(importGraph()), *(IconManager::getInstance()->getIcon("import")));
    QAction *actionExport  = comm.addAction("Export", SLOT(exportGraph()), *(IconManager::getInstance()->getIcon("export")));
    QAction *actionReset   = comm.addAction("Reset", SLOT(resetGraph()), *(IconManager::getInstance()->getIcon("reset")));
    QAction *actionLayout  = comm.addAction("Layout", SLOT(changeLayout()), *(IconManager::getInstance()->getIcon("layout")), this);
    QAction *actionReloadPropertyDialog = comm.addAction("Reload Command Panel", SLOT(reloadPropertyDialog()), *(IconManager::getInstance()->getIcon("reload")), WidgetManager::getInstance()->getGraphManager());

    contextMenu.addAction(actionImport);
    contextMenu.addAction(actionExport);
    contextMenu.addSeparator();
    contextMenu.addAction(actionRefresh);
    contextMenu.addAction(actionShuffle);
    contextMenu.addAction(actionReset);
    contextMenu.addAction(actionLayout);
    if(!WidgetManager::getInstance()->getPropertyDialog()->isRunning())
    {
        contextMenu.addSeparator();
        contextMenu.addAction(actionReloadPropertyDialog);
    }
    contextMenu.exec(mapToGlobal(pos));
}

void LayerViewWidget::setGraph(graph_analysis::BaseGraph::Ptr graph)
{
    mpGraph = graph;
    mpSubGraph = mGraphView.apply(mpGraph);
    mFiltered = true;

    mpSubGraph->enableAllVertices();
    mpSubGraph->enableAllEdges();
    reset(true);
    updateFromGraph();
}

void LayerViewWidget::changeLayout()
{
    updateStatus(std::string("changing layers graph view layout..."));
    bool ok;
    QStringList options;
    std::set<std::string> supportedLayouts = mpGVGraph->getSupportedLayouts();
//    options << tr("circo") << tr("dot") << tr("fdp") << tr("neato") << tr("osage") << tr("sfdp") << tr("twopi"); // [circo, dot, fdp, neato, osage, sfdp, twopi]
    foreach(std::string supportedLayout, supportedLayouts)
    {
        options << tr(supportedLayout.c_str());
    }
    QString layout = QInputDialog::getItem(this, tr("Input New Layout"),
                                         tr("select a layout:"), options,
                                         0, false, &ok);
    if (ok && !layout.isEmpty())
    {
        std::string desiredLayout = layout.toStdString();
        reset(true /*keepData*/);
        setLayout(QString(desiredLayout.c_str()));
        updateStatus(std::string("Changed layers graph view layout to '") + desiredLayout + "'!", GraphManager::TIMEOUT);
    }
    else
    {
        updateStatus(std::string("Failed to change layers graph view layout: aborted by user!"), GraphManager::TIMEOUT);
    }
}

void LayerViewWidget::reset(bool keepData)
{
    clear();

    if(mpGVGraph)
    {
        delete mpGVGraph;
    }
    mpGVGraph = new io::GVGraph(mpGraph, "GVLayerViewWidget");
}

void LayerViewWidget::clear()
{
    // TO-DO: add filtering clearing
    if(mpGVGraph)
    {
        mpGVGraph->clearEdges();
        mpGVGraph->clearNodes();
    }

    mNodeItemMap.clear();
    mEdgeItemMap.clear();
    scene()->clear();
}
void LayerViewWidget::refresh(bool status)
{
    if(status)
    {
        updateStatus(std::string("refreshing layers graph view..."));
    }
    reset(true /*keepData*/);
    updateFromGraph();
    update();
    if(status)
    {
        updateStatus(std::string("Refreshed layers graph view!"), GraphManager::TIMEOUT);
    }
}

void LayerViewWidget::enableVertex(graph_analysis::Vertex::Ptr vertex)
{
    mpSubGraph->enable(vertex);
    LOG_DEBUG_S << "Enabled a vertex of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void LayerViewWidget::enableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->enable(edge);
    LOG_DEBUG_S << "Enabled an edge of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void LayerViewWidget::disableVertex(graph_analysis::Vertex::Ptr vertex)
{
    mpSubGraph->disable(vertex);
    LOG_DEBUG_S << "Disabled vertex '" << vertex->getLabel() << "' of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void LayerViewWidget::disableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->disable(edge);
    LOG_DEBUG_S << "Disabled edge '" << edge->getLabel() << "' of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void LayerViewWidget::updateFromGraph()
{
    if(mpGraph->empty())
    {
        LOG_WARN_S << "graph_analysis::gui::LayerViewWidget::updateFromGraph was called while mpGraph was still empty";
        return;
    }

    // setting up custom regexp filters
    std::set<std::string> filters;
    PropertyDialog *propertyDialog = WidgetManager::getInstance()->getPropertyDialog();
    if(propertyDialog)
    {
        FilterManager::Filters manager_filters = propertyDialog->getFilterManager()->getFilters();
        foreach(FilterItem *item, manager_filters)
        {
            filters.insert(item->getLabel().toStdString());
        }
    }

    using namespace graph_analysis;
    foreach(std::string regexp, filters)
    {
        try
        {
            LOG_INFO_S << "Add regexp: " << regexp << " to filter";
            filters::VertexRegexFilter::Ptr contentFilter(new filters::VertexRegexFilter(regexp, filters::CONTENT, false));
            mpVertexFilter->add(contentFilter);
        }
        catch(boost::regex_error e)
        {
            LOG_ERROR_S << "graph_analysis::gui::LayerViewWidget::updateFromGraph: skipping regex '" << regexp << "'. Caught Regex error: " << e.what();
        }
    }

//    mGraphView.setVertexFilter(mpVertexFilter);
//    mpSubGraph = mGraphView.apply(mpGraph);
//    mFiltered = true;
    // finished setting up the custom regexp filters

    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // Check on active filter
        if(/*mFiltered && */!mpVertexFilter->permits(vertex))
        {
            LOG_DEBUG_S << "graph_analysis::gui::LayerViewWidget: Custom-Regex-Filtered out vertex: " << vertex->toString();
            continue;
        } else {
            LOG_DEBUG_S << "graph_analysis::gui::LayerViewWidget: Custom-Regex-Filtered kept vertex: " << vertex->toString();
        }

        if(mFiltered && !mpSubGraph->enabled(vertex))
        {
            LOG_DEBUG_S << "graph_analysis::gui::LayerViewWidget: Filtered out vertex: " << vertex->toString();
            continue;
        }

        if(mNodeItemMap.count(vertex))
        {
            continue;
        }

        if(toggledOut(vertex))
        {
            continue;
        }

        // Registering new node items
        NodeItem* nodeItem = NodeTypeManager::getInstance()->createItem(this, vertex, layeritem::Resource::sType);
        mNodeItemMap[vertex] = nodeItem;
        scene()->addItem(nodeItem);
        mpGVGraph->addNode(vertex);
    }
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        // Check on active filter
        if(mFiltered && !mpSubGraph->enabled(edge))
        {
            LOG_DEBUG_S << "graph_analysis::gui::LayerViewWidget: Filtered out edge: " << edge->toString();
            continue;
        }

        if(mEdgeItemMap.count(edge))
        {
            continue;
        }

        // Registering new edge items
        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        if(
            toggledOut(source)
                ||
            toggledOut(target)
        )
        {
            continue;
        }

        NodeItem* sourceNodeItem = mNodeItemMap[ source ];
        NodeItem* targetNodeItem = mNodeItemMap[ target ];

        if(!sourceNodeItem || !targetNodeItem)
        {
            continue;
        }

        EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, targetNodeItem, edge, LAYER_EDGE_TYPE);
        mEdgeItemMap[edge] = edgeItem;

        scene()->addItem(edgeItem);
        mpGVGraph->addEdge(edge);
    }

    if(mLayout.toLower() != "force")
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        // filtering out "circo" layouting engine on filtered graphs (using "dot" instead)
        bool fakeLayout =   // when true, will replace circo with the default (internally, "dot")
                            "circo" == mLayout.toLower()
                                &&
                            (
                                !mFeatureLayerToggle
                                    ||
                                !mClusterLayerToggle
                            )
                            ;
        LOG_INFO_S << "GV started layouting the graph. This can take a while ...";
        base::Time start = base::Time::now();
        mpGVGraph->applyLayout(fakeLayout ? "dot" : mLayout.toStdString());
        base::Time delay = base::Time::now() - start;
        QApplication::restoreOverrideCursor();
        LOG_INFO_S << "GV layouted the graph after " << delay.toSeconds();

        {
            using namespace graph_analysis::io;
            std::vector<GVNode> nodes = mpGVGraph->nodes();
            std::vector<GVNode>::const_iterator cit = nodes.begin();
            for(; cit != nodes.end(); ++cit)
            {
                GVNode gvNode = *cit;
                NodeItem* nodeItem = mNodeItemMap[gvNode.getVertex()];

                if(!nodeItem)
                {
                    LOG_WARN_S << "NodeItem: mapped from " <<  gvNode.getVertex()->toString() << "is null";
                    continue;
                }

                nodeItem->setPos(mScaleFactor * gvNode.x(), mScaleFactor * gvNode.y());
            }
        }

//        {
//            using namespace graph_analysis::io;
//            std::vector<GVEdge> edges = mpGVGraph->edges();
//            std::vector<GVEdge>::const_iterator cit = edges.begin();
//            for(; cit != edges.end(); ++cit)
//            {
//                GVEdge gvEdge = *cit;
//                EdgeItem* edgeItem = mEdgeItemMap[ gvEdge.getEdge() ];
//                edgeItem->setPainterPath( edge.path );
//            }
//        }
    }
}

void LayerViewWidget::addVertex(Vertex::Ptr vertex)
{
    mpGraph->addVertex(vertex);
}

void LayerViewWidget::addEdge(Edge::Ptr edge)
{
    mpGraph->addEdge(edge);
}

void LayerViewWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to enable Panning
        setDragMode(ScrollHandDrag);
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mousePressEvent(&fake);
    }
    else QGraphicsView::mousePressEvent(event);
}

void LayerViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to end Panning
        setDragMode(NoDrag);
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mouseReleaseEvent(&fake);
    }
    else QGraphicsView::mouseReleaseEvent(event);
}

void LayerViewWidget::itemMoved()
{
    if (!mTimerId)
    {
        mTimerId = startTimer(1000 / 25);
    }
}

void LayerViewWidget::keyPressEvent(QKeyEvent *event)
{
    // check for a combination of user presses
    Qt::KeyboardModifiers modifiers = event->modifiers();

    if(modifiers & Qt::ControlModifier)
    {
        switch (event->key())
        {
            case Qt::Key_Q: // CTRL+Q and CTRL+W terminate the application
            case Qt::Key_W:
                exit(0);
            break;

            case Qt::Key_Plus: // CTRL+ zooms in
                zoomIn();
            break;

            case Qt::Key_Minus: // CTRL+- zooms out
                zoomOut();
            break;

            case Qt::Key_Left: // CTRL+LeftArrow rotates the view counterclockwise
                rotate(qreal(-1.13));
            break;

            case Qt::Key_Right: // CTRL+RightArrow rotates the view clockwise
                rotate(qreal( 1.13));
            break;

            case Qt::Key_R: // CTRL+R deletes the graph (it first prompts again the user)
                WidgetManager::getInstance()->getComponentEditorWidget()->resetGraph();
            break;

            case Qt::Key_E: // CTRL+S (save) or CTRL+E (export graph) saves the graph to file
            case Qt::Key_S:
                WidgetManager::getInstance()->getComponentEditorWidget()->exportGraph();
            break;

            case Qt::Key_I: // CTRL+O (open) or CTRL+I (input graph)  or CTRL+L (load graph) opens a graph from file
            case Qt::Key_O:
            case Qt::Key_L:
                WidgetManager::getInstance()->getComponentEditorWidget()->importGraph();
            break;

            case Qt::Key_P: // CTRL+P reloads the property dialog (if it is currently not running)
                if(!WidgetManager::getInstance()->getPropertyDialog()->isRunning())
                {
                    WidgetManager::getInstance()->getGraphManager()->reloadPropertyDialog();
                }
            break;
        }
    }
    else
    {
        switch (event->key())
        {
        //case Qt::Key_Up:
        //    break;
        //case Qt::Key_Down:
        //    break;
        //case Qt::Key_Left:
        //    break;
        //case Qt::Key_Right:
        //    break;
        case Qt::Key_Plus: // '+' zooms-in
            zoomIn();
        break;
        case Qt::Key_Minus: // '-' zooms-out
            zoomOut();
        break;
        case Qt::Key_Space: // Space, newline and 'R'/'r' refresh the view
        case Qt::Key_Enter:
        case Qt::Key_R:
                refresh();
        break;

        case Qt::Key_S: // 'S'/'s' shuffle the nodes
            shuffle();
        break;

        case Qt::Key_L: // 'L'/'l' changes the layout
            changeLayout();
        break;
        }
    }

    QGraphicsView::keyPressEvent(event);
}

void LayerViewWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if(mLayout.toLower() == "force")
    {
        QList<NodeItem* > nodes;
        foreach (QGraphicsItem *item, scene()->items())
        {
            if (NodeItem* node = qgraphicsitem_cast<NodeItem* >(item))
            {
                nodes << node;
            }
        }

        foreach (NodeItem* node, nodes)
        {
            node->calculateForces();
        }

        bool itemsMoved = false;
        foreach (NodeItem* node, nodes)
        {
            if (node->advance())
                itemsMoved = true;
        }

        if (!itemsMoved)
        {
            killTimer(mTimerId);
            mTimerId = 0;
        }
    }
}

#ifndef QT_NO_WHEELEVENT
void LayerViewWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, - event->delta() / 240.0));
}
#endif

void LayerViewWidget::drawBackground(QPainter *painter, const QRectF& rect)
{
    Q_UNUSED(rect);

    //// Shadow
//    QRectF sceneRect = this->sceneRect();
    //QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    //QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    //if (rightShadow.intersects(rect) || rightShadow.contains(rect))
    //    painter->fillRect(rightShadow, Qt::darkGray);
    //if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
    //    painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    //QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    //gradient.setColorAt(0, Qt::white);
    //gradient.setColorAt(1, Qt::lightGray);
    //painter->fillRect(rect.intersected(sceneRect), gradient);
    //painter->setBrush(Qt::NoBrush);
    //painter->drawRect(sceneRect);

    //// Text
    //QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
    //                sceneRect.width() - 4, sceneRect.height() - 4);
    //QString message(tr("Click and drag the nodes around, and zoom with the mouse "
    //                   "wheel or the '+' and '-' keys"));

    //QFont font = painter->font();
    //font.setBold(true);
    //font.setPointSize(14);
    //painter->setFont(font);
    //painter->setPen(Qt::lightGray);
    //painter->drawText(textRect.translated(2, 2), message);
    //painter->setPen(Qt::black);
    //painter->drawText(textRect, message);
}

void LayerViewWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
    {
        return;
    }
    scale(scaleFactor, scaleFactor);
    std::string status_msg = scaleFactor > 1. ? "Zoomed-in" : "Zoomed-out";
    updateStatus(status_msg, GraphManager::TIMEOUT);
}

void LayerViewWidget::shuffle()
{
    updateStatus(std::string("shuflling all the nodes in the layers graph view..."));
    foreach (QGraphicsItem *item, scene()->items())
    {
        if (qgraphicsitem_cast<NodeItem* >(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
    updateStatus(std::string("Shuflled all nodes in the layers graph view!"), GraphManager::TIMEOUT);
}

void LayerViewWidget::zoomIn()
{
    scaleView(qreal(1.13));
}

void LayerViewWidget::zoomOut()
{
    scaleView(1 / qreal(1.13));
}

void LayerViewWidget::setLayout(QString layoutName)
{
    mLayout = layoutName;
    updateFromGraph();
}

void LayerViewWidget::toggleFeatureLayer(bool toggle)
{
    updateStatus(std::string("toggling the features layer to ") + (toggle ? "true" : "false" ) + "...");
    mFeatureLayerToggle = toggle;
    refresh(false);
    updateStatus(std::string("Toggled the features layer to ") + (toggle ? "true" : "false" ) + "!", GraphManager::TIMEOUT);
}

void LayerViewWidget::toggleClusterLayer(bool toggle)
{
    updateStatus(std::string("toggling the clusters layer to ") + (toggle ? "true" : "false" ) + "...");
    mClusterLayerToggle = toggle;
    refresh(false);
    updateStatus(std::string("Toggled the clusters layer to ") + (toggle ? "true" : "false" ) + "!", GraphManager::TIMEOUT);
}

inline void LayerViewWidget::updateStatus(const std::string& message, int timeout)
{
    WidgetManager::getInstance()->getGraphManager()->updateStatus(message, timeout);
}

inline bool LayerViewWidget::toggledOut(graph_analysis::Vertex::Ptr vertex)
{
    bool result =   (!mFeatureLayerToggle && graph_analysis::PortVertex::vertexType() == vertex->getClassName())
                        ||
                    (!mFeatureLayerToggle && graph_analysis::InputPortVertex::vertexType() == vertex->getClassName())
                        ||
                    (!mFeatureLayerToggle && graph_analysis::OutputPortVertex::vertexType() == vertex->getClassName())
                        ||
                    (!mFeatureLayerToggle && graph_analysis::PropertyVertex::vertexType() == vertex->getClassName())
                        ||
                    (!mFeatureLayerToggle && graph_analysis::OperationVertex::vertexType() == vertex->getClassName())
                        ||
                    (!mClusterLayerToggle && graph_analysis::ClusterVertex::vertexType() == vertex->getClassName())
                ;
    return result;
}

} // end namespace gui
} // end namespace graph_analysis
