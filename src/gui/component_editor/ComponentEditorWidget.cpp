#include "ComponentEditorWidget.hpp"
#include <set>
#include <math.h>
#include <sstream>
#include <exception>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>

#include <QDir>
#include <QTime>
#include <QMenu>
#include <QLabel>
#include <QAction>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSignalMapper>

#include <base/Time.hpp>
#include <base/Logging.hpp>
#include <boost/lexical_cast.hpp>

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>
#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <graph_analysis/gui/EdgeItem.hpp>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/IconManager.hpp>
#include <graph_analysis/gui/GraphWidgetManager.hpp>
#include <graph_analysis/gui/WidgetManager.hpp>
#include <graph_analysis/gui/NodeItemTypeManager.hpp>
#include <graph_analysis/gui/EdgeItemTypeManager.hpp>
#include <graph_analysis/gui/ActionCommander.hpp>
#include <graph_analysis/gui/dialogs/SwapFeaturesDialog.hpp>
#include <graph_analysis/gui/dialogs/RenameFeatureDialog.hpp>
#include <graph_analysis/gui/dialogs/AddGraphElement.hpp>
#include <graph_analysis/gui/dialogs/PropertyDialog.hpp>
#include <graph_analysis/gui/component_editor/Port.hpp>
#include <graph_analysis/gui/component_editor/InputPort.hpp>
#include <graph_analysis/gui/component_editor/OutputPort.hpp>
#include <graph_analysis/gui/component_editor/Cluster.hpp>
#include <graph_analysis/gui/component_editor/Property.hpp>
#include <graph_analysis/gui/component_editor/Operation.hpp>
#include <graph_analysis/gui/component_editor/PortConnection.hpp>

#include <graph_analysis/gui/items/EdgeLabel.hpp>
#include <graph_analysis/gui/items/Feature.hpp>

// comment out to toggle-out focused node be re-doule-clicking it; leave untouched to be able to cancel node focusing by double-clicking the background
#define CLEAR_BY_BACKGROUND

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

ComponentEditorWidget::ComponentEditorWidget(QWidget *parent)
    : LayerViewWidget(parent)
    , mpLastFocusedNodeItem(0)
{
    registerTypes();

    // Add seed for force layout
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    mScaleFactor *= 1.69;

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    setCacheMode(CacheBackground);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Component Editor"));
    // Setting up filtering
    mGraphView.setVertexFilter(mpVertexFilter);
    mGraphView.setEdgeFilter(mpEdgeFilter);

    // setting up the context menu
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint &)));
}

ComponentEditorWidget::~ComponentEditorWidget()
{}

void ComponentEditorWidget::registerTypes()
{
    {
        VertexTypeManager* manager = VertexTypeManager::getInstance();
        manager->registerType(Port::vertexType(), Vertex::Ptr(new Port()));
        manager->registerType(InputPort::vertexType(),  Vertex::Ptr (new InputPort()));
        manager->registerType(OutputPort::vertexType(), Vertex::Ptr(new OutputPort()));
        manager->registerType(Cluster::vertexType(),    Vertex::Ptr(new Cluster()));
        manager->registerType(Property::vertexType(),   Vertex::Ptr(new Property()));
        manager->registerType(Operation::vertexType(),  Vertex::Ptr (new Operation()));
    }

    {
        EdgeTypeManager* manager = EdgeTypeManager::getInstance();
        manager->registerType(PortConnection::edgeType(), Edge::Ptr(new PortConnection()));
    }
}

void ComponentEditorWidget::showContextMenu(const QPoint& pos)
{
    // Cache focused node item -- since when the context
    // menu is started the NodeItem will loose the focuse
    mpLastFocusedNodeItem = getFocusedNodeItem();

    ActionCommander comm(this);
    QMenu contextMenu(tr("Context menu"));

    QAction *actionAddNode = comm.addAction("Add Node", SLOT(addVertexDialog()), *(IconManager::getInstance()->getIcon("addNode")));
    QAction *actionEdit = comm.addAction("Edit", SLOT(editElementDialog()), *(IconManager::getInstance()->getIcon("edit_mode_white")));
    QAction *actionRemoveElement  = comm.addAction("Remove", SLOT(removeElementDialog()), *(IconManager::getInstance()->getIcon("remove")));
    QAction *actionAddFeature     = comm.addAction("Add Feature", SLOT(addFeatureDialog()), *(IconManager::getInstance()->getIcon("addFeature")));
    QAction *actionRefresh = comm.addAction("Refresh", SLOT(refresh()), *(IconManager::getInstance()->getIcon("refresh")));
    QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffle()), *(IconManager::getInstance()->getIcon("shuffle")));
    //QAction *actionImport = comm.addAction("Import", SLOT(importGraph()), *(IconManager::getInstance()->getIcon("import")));
    //QAction *actionExport = comm.addAction("Export", SLOT(exportGraph()), *(IconManager::getInstance()->getIcon("export")));
    QAction *actionLayout = comm.addAction("Layout", SLOT(selectLayoutDialog()), *(IconManager::getInstance()->getIcon("layout")));
    QAction *actionClear  = comm.addAction("Clear", SLOT(clearDialog()), *(IconManager::getInstance()->getIcon("reset")));

    contextMenu.addAction(actionAddNode);
    if(getGraphWidgetManager()->getMode() == GraphWidgetManager::EDIT_MODE)
    {
        contextMenu.addAction(actionAddFeature);
        contextMenu.addAction(actionRemoveElement);
        contextMenu.addAction(actionEdit);
        contextMenu.addAction(actionClear);
        contextMenu.addSeparator();
    }

    contextMenu.addAction(actionRefresh);
    contextMenu.addAction(actionLayout);
    contextMenu.addAction(actionShuffle);
    contextMenu.addSeparator();

    contextMenu.exec(mapToGlobal(pos));
}

void ComponentEditorWidget::addFeatureDialog()
{
    if(!mpLastFocusedNodeItem)
    {
        LOG_WARN_S << "No current node to add feature on";
        updateStatus("No current node to add feature on");
        return;
    }

    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
    QStringList supportedTypes;
    std::set<std::string>::const_iterator cit = types.begin();
    for(; cit != types.end(); ++cit)
    {
        // TODO filter out actual feature types for the given node
        supportedTypes << QString(cit->c_str());
    }

    dialogs::AddGraphElement graphElementDialog(supportedTypes, this);
    graphElementDialog.setWindowTitle("Add feature");
    graphElementDialog.exec();
    if(graphElementDialog.result() == QDialog::Accepted)
    {

        Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(graphElementDialog.getType().toStdString(),
                graphElementDialog.getLabel().toStdString());

        Edge::Ptr edge(new Edge(mpLastFocusedNodeItem->getVertex(), vertex));
        edge->setLabel("hasFeature");
        graph()->addEdge(edge);
        updateStatus("Added feature '" + vertex->toString() + "' of type '" + vertex->getClassName() + "'", GraphWidgetManager::TIMEOUT);
        refresh();
    } else {
        updateStatus("Adding feature aborted by user");
    }
}

void ComponentEditorWidget::enableVertex(graph_analysis::Vertex::Ptr vertex)
{
    mpSubGraph->enable(vertex);
    LOG_DEBUG_S << "Enabled vertex '" << vertex->getLabel() << "' of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void ComponentEditorWidget::enableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->enable(edge);
    LOG_DEBUG_S << "Enabled edge '" << edge->getLabel() << "' of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void ComponentEditorWidget::disableVertex(graph_analysis::Vertex::Ptr vertex)
{
    mpSubGraph->disable(vertex);
    LOG_DEBUG_S << "Disabled vertex '" << vertex->getLabel() << "' of ID: " << mpSubGraph->getBaseGraph()->getVertexId(vertex);
}
void ComponentEditorWidget::disableEdge(graph_analysis::Edge::Ptr edge)
{
    mpSubGraph->disable(edge);
    LOG_DEBUG_S << "Disabled edge '" << edge->getLabel() << "' of ID:  " << mpSubGraph->getBaseGraph()->getEdgeId(edge);
}

void ComponentEditorWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
}

void ComponentEditorWidget::itemMoved()
{
    if (!mTimerId)
    {
        mTimerId = startTimer(1000 / 25);
    }
}

void ComponentEditorWidget::keyPressEvent(QKeyEvent *event)
{
//    // check for a keys combination
//    Qt::KeyboardModifiers modifiers = event->modifiers();
//
//    if(modifiers & Qt::ControlModifier) // key combinations while holding 'CTRL' pressed
//    {
//        switch(event->key())
//        {
//            case Qt::Key_Q: // CTRL+Q and CTRL+W terminate the application
//            case Qt::Key_W:
//                exit(0);
//            break;
//
//            case Qt::Key_Plus: // CTRL+ zooms in
//                zoomIn();
//            break;
//
//            case Qt::Key_Minus: // CTRL+- zooms out
//                zoomOut();
//            break;
//
//            case Qt::Key_R: // CTRL+R deletes the graph (it first prompts again the user)
//                resetGraph();
//            break;
//
//            case Qt::Key_E: // CTRL+S (save) or CTRL+E (export graph) saves the graph to file
//            case Qt::Key_S:
//                exportGraph();
//            break;
//
//            case Qt::Key_A: // CTRL+A prompts the user to add a node
//                if(!mDragDrop)
//                {
//                    addNodeAdhoc();
//                }
//            break;
//
//            case Qt::Key_I: // CTRL+O (open) or CTRL+I (input graph)  or CTRL+L (load graph) opens a graph from file
//            case Qt::Key_O:
//            case Qt::Key_L:
//                importGraph();
//            break;
//
//            case Qt::Key_P: // CTRL+P reloads the property dialog (if it is currently not running)
//                if(!WidgetManager::getInstance()->getPropertyDialog()->isRunning())
//                {
//                    WidgetManager::getInstance()->getGraphWidgetManager()->reloadPropertyDialog();
//                }
//            break;
//
//            case Qt::Key_Left: // CTRL+LeftArrow rotates the view counterclockwise
//                if(!mDragDrop)
//                {
//                    rotate(qreal(-1.13));
//                }
//            break;
//
//            case Qt::Key_Right: // CTRL+RightArrow rotates the view clockwise
//                if(!mDragDrop)
//                {
//                    rotate(qreal( 1.13));
//                }
//            break;
//
//            case Qt::Key_CapsLock: // CTRL+CapsLock or CTRL+D toggles the active mode (drag-n-drop mode v. move-around mode)
//            case Qt::Key_D:
//                mDragDrop ? unsetDragDrop() : setDragDrop();
//            break;
//        }
//    }
//    else if(!mDragDrop) // simple keys (move-around mode only!)
//    {
//        switch(event->key())
//        {
//        //case Qt::Key_Up:
//        //    break;
//        //case Qt::Key_Down:
//        //    break;
//        //case Qt::Key_Left:
//        //    break;
//        //case Qt::Key_Right:
//        //    break;
//        case Qt::Key_Plus: // '+' zooms-in
//            zoomIn();
//        break;
//
//        case Qt::Key_Minus: // '-' zooms-out
//            zoomOut();
//        break;
//
//        case Qt::Key_Space: // Space, newline and 'R'/'r' refresh the view
//        case Qt::Key_Enter:
//        case Qt::Key_R:
//            refresh();
//        break;
//
//        case Qt::Key_S: // 'S'/'s' shuffle the nodes
//            shuffle();
//        break;
//
//        case Qt::Key_L: // 'L'/'l' changes the layout
//            changeLayout();
//        break;
//        }
//    }
//
//    switch(event->key()) // simple keys (permanent)
//    {
//    case Qt::Key_Escape: // clears node and edge focus
//        clearFocus();
//    break;
//    }

    GraphWidget::keyPressEvent(event);

}

void ComponentEditorWidget::timerEvent(QTimerEvent *event)
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
void ComponentEditorWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, - event->delta() / 240.0));
}
#endif

void ComponentEditorWidget::drawBackground(QPainter *painter, const QRectF& rect)
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

void ComponentEditorWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
    {
        return;
    }
    scale(scaleFactor, scaleFactor);
    std::string status_msg = scaleFactor > 1. ? "Zoomed-in" : "Zoomed-out";
    updateStatus(status_msg);
}

void ComponentEditorWidget::shuffle()
{
    updateStatus("Shuffeling all nodes ...");
    foreach (QGraphicsItem *item, scene()->items())
    {
        if (qgraphicsitem_cast<NodeItem* >(item))
        {
            item->setPos(-150 * mScaleFactor + mScaleFactor * (qrand() % 300), -150 * mScaleFactor + mScaleFactor * (qrand() % 300));
        }
    }
    updateStatus("Done shuffeling all nodes");
}

void ComponentEditorWidget::zoomIn()
{
    scaleView(qreal(1.13));
}

void ComponentEditorWidget::zoomOut()
{
    scaleView(1 / qreal(1.13));
}

void ComponentEditorWidget::syncEdgeItemMap(graph_analysis::Edge::Ptr concernedEdge)
{
    EdgeItemMap::iterator it = mEdgeItemMap.find(concernedEdge);
    if(mEdgeItemMap.end() != it)
    {
        mEdgeItemMap.erase(it);
    }
}

void ComponentEditorWidget::updateLayout()
{
    mFeatureMap.clear();

    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // Check on active filter
        if(mFiltered && !mpSubGraph->enabled(vertex))
        {
            LOG_DEBUG_S << "graph_analysis::gui::ComponentEditorWidget: Filtered out vertex: " << vertex->toString();
            continue;
        }

        if(mNodeItemMap.count(vertex))
        {
            continue;
        }

        if(Cluster::vertexType() == vertex->getClassName())
        {
            // Registering new Cluster node items only
            NodeItem* nodeItem = NodeItemTypeManager::getInstance()->createItem(this, vertex);
            mNodeItemMap[vertex] = nodeItem;
            scene()->addItem(nodeItem);
            mpLayoutingGraph->addVertex(vertex);
            mpGVGraph->addNode(vertex);

            LOG_DEBUG_S << "Adding vertex of type: " << vertex->getClassName();
        } 
    }

    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        std::string sourceClassName = source->getClassName();
        std::string targetClassName = target->getClassName();

        if(OutputPort::vertexType() == sourceClassName 
                && InputPort::vertexType() == targetClassName)
        {
            // physical edge - processing deflected until after all features will have been registered
            continue;
        } else if (Cluster::vertexType() == sourceClassName
                        && ( InputPort::vertexType() == targetClassName
                                || OutputPort::vertexType() == targetClassName
                                || Property::vertexType() == targetClassName
                                || Operation::vertexType() == targetClassName
                           ))
        {
            // semantical edge: links a cluster vertex to one of its features
            NodeItem* sourceNodeItem = mNodeItemMap[ source ];
            if(!sourceNodeItem)
            {
                continue;
            }

            items::Feature* feature = new items::Feature(target, this);
            sourceNodeItem->addFeature(feature);
            mFeatureMap[target] = feature;
        } else
        {
            //// invalid edge
            std::string error_msg = std::string("graph_analysis::ComponentEditorWidget::updateLayout: found invalid edge from source vertex '") +
                                        source->toString() + "' of type '" + sourceClassName + "' to target vertex '" +
                                        target->toString() + "' of type '" + targetClassName + "'!";
            LOG_WARN_S << error_msg;
        //    throw std::runtime_error(error_msg);
        }
    }

    // re-iterating the edges for rendering the physical ones
    edgeIt = mpGraph->getEdgeIterator();
    std::vector<Edge::Ptr> markedForRemoval;
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        PortConnection::Ptr connection = dynamic_pointer_cast<PortConnection>(edge);
        if(connection)
        {
            LOG_DEBUG_S << "Visualizing port connection";
            Vertex::Ptr source = edge->getSourceVertex();
            Vertex::Ptr target = edge->getTargetVertex();
            if(!source || !target)
            {
                LOG_INFO_S << "Failed to identify target or source feature from given vertices. "
                    << " Marking edge for removal";
                markedForRemoval.push_back(edge);
                continue;
            }

            items::Feature* sourceFeature = getFeature(source);
            items::Feature* targetFeature = getFeature(target);

            if(!sourceFeature || !targetFeature)
            {
                LOG_INFO_S << "Failed to identify target or source feature from given vertices. "
                    << " Marking edge for removal";
                markedForRemoval.push_back(edge);
                continue;
            } else {
                EdgeItem* edgeItem = EdgeItemTypeManager::getInstance()->createItem(this, sourceFeature, targetFeature, edge, "default");
                mEdgeItemMap[edge] = edgeItem;
                scene()->addItem(edgeItem);

                mpLayoutingGraph->addEdge(connection);
            }
        }
    }

    // Cleanup invalid edges if needed
    while(!markedForRemoval.empty())
    {
        mpGraph->removeEdge(markedForRemoval.at(0));
    }

}

items::Feature* ComponentEditorWidget::getFeature(const GraphElement::Ptr& element)
{
    std::map<GraphElement::Ptr, items::Feature*>::iterator it = mFeatureMap.find(element);
    if(it != mFeatureMap.end())
    {
        return it->second;
    }

    return NULL;
}

} // end namespace gui
} // end namespace graph_analysis
