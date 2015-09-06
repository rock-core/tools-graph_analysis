#include "ComponentEditorWidget.hpp"
#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "IconManager.hpp"
#include "GraphWidgetManager.hpp"
#include "WidgetManager.hpp"
#include "NodeTypeManager.hpp"
#include "EdgeTypeManager.hpp"
#include "ActionCommander.hpp"
#include "SwapFeaturesDialog.hpp"
#include "RenameFeatureDialog.hpp"
#include "AddGraphElement.hpp"

#include <set>
#include <math.h>
#include <sstream>
#include <QDir>
#include <QTime>
#include <QMenu>
#include <QLabel>
#include <sstream>
#include <QAction>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSignalMapper>
#include <boost/regex.hpp>
#include <base/Logging.hpp>
#include <boost/lexical_cast.hpp>

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/PortVertex.hpp>
#include <graph_analysis/InputPortVertex.hpp>
#include <graph_analysis/OutputPortVertex.hpp>
#include <graph_analysis/ClusterVertex.hpp>
#include <graph_analysis/PropertyVertex.hpp>
#include <graph_analysis/OperationVertex.hpp>

#include <graph_analysis/Filter.hpp>
#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>
#include <graph_analysis/gui/items/EdgeLabel.hpp>
#include <graph_analysis/gui/items/Feature.hpp>

#include <exception>
#include <boost/foreach.hpp>
#include <base/Time.hpp>

// comment out to toggle-out focused node be re-doule-clicking it; leave untouched to be able to cancel node focusing by double-clicking the background
#define CLEAR_BY_BACKGROUND

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

ComponentEditorWidget::ComponentEditorWidget(QWidget *parent)
    : LayerViewWidget(parent)
    , mVertexFocused(false)
    , mEdgeFocused(false)
    , mDragDrop(false)
    , mFocusedNodeItem(0)
{
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

void ComponentEditorWidget::showContextMenu(const QPoint& pos)
{
    mFocusedNodeItem = getFocusedNodeItem();

    ActionCommander comm(this);
    QPoint position = mapTo(this, pos);
    QMenu contextMenu(tr("Context menu"), this);

    //QAction *actionChangeEdgeLabel = comm.addAction("Rename Edge", SLOT(changeSelectedEdgeLabel()), *(IconManager::getInstance()->getIcon("label")));
    //QAction *actionRemoveEdge  = comm.addAction("Remove Edge", SLOT(removeSelectedEdge()), *(IconManager::getInstance()->getIcon("remove")));
    //QAction *actionChangeLabel = comm.addAction("Rename Node", SLOT(changeSelectedVertexLabel()), *(IconManager::getInstance()->getIcon("label")));
    QAction *actionRemoveNode  = comm.addAction("Remove Node", SLOT(removeFocusedVertex()), *(IconManager::getInstance()->getIcon("remove")));
    QAction *actionAddFeature     = comm.addAction("Add Feature", SLOT(addFeatureDialog()), *(IconManager::getInstance()->getIcon("addFeature")));
    //QAction *actionSwapFeatures   = comm.addAction("Swap Features", SLOT(swapFeaturesSelected()), *(IconManager::getInstance()->getIcon("swap")));
    //QAction *actionRenameFeature  = comm.addAction("Rename a Feature", SLOT(renameFeatureSelected()), *(IconManager::getInstance()->getIcon("featureLabel")));
    //QAction *actionRemoveFeature  = comm.addAction("Remove a Feature", SLOT(removeFeatureSelected()), *(IconManager::getInstance()->getIcon("remove")));
    //QAction *actionRemoveFeatures = comm.addAction("Remove Features", SLOT(removeFeaturesSelected()), *(IconManager::getInstance()->getIcon("removeAll")));
    //QAction *actionAddNode = comm.addMappedAction("Add Node", SLOT(addNodeAdhoc(QObject*)), (QObject*)&position, *(IconManager::getInstance()->getIcon("addNode")));
    //QAction *actionRefresh = comm.addAction("Refresh", SLOT(refresh()), *(IconManager::getInstance()->getIcon("refresh")));
    //QAction *actionShuffle = comm.addAction("Shuffle", SLOT(shuffle()), *(IconManager::getInstance()->getIcon("shuffle")));
    //QAction *actionImport = comm.addAction("Import", SLOT(importGraph()), *(IconManager::getInstance()->getIcon("import")));
    //QAction *actionExport = comm.addAction("Export", SLOT(exportGraph()), *(IconManager::getInstance()->getIcon("export")));
    //QAction *actionReset  = comm.addAction("Reset", SLOT(resetGraph()), *(IconManager::getInstance()->getIcon("reset")));
    //QAction *actionLayout = comm.addAction("Layout", SLOT(changeLayout()), *(IconManager::getInstance()->getIcon("layout")));
    //QAction *actionReloadPropertyDialog = comm.addAction("Reload Command Panel", SLOT(reloadPropertyDialog()), *(IconManager::getInstance()->getIcon("reload")), WidgetManager::getInstance()->getGraphWidgetManager());

    //// (conditionally) adding the actions to the context menu
    //if(mEdgeSelected)
    //{
    //    contextMenu.addAction(actionChangeEdgeLabel);
    //    contextMenu.addAction(actionRemoveEdge);
    //}
    //if(mVertexSelected)
    //{
    //    if(mEdgeSelected)
    //    {
    //        contextMenu.addSeparator();
    //    }
    //    contextMenu.addAction(actionChangeLabel);
        contextMenu.addAction(actionAddFeature);
    //    contextMenu.addAction(actionSwapFeatures);
    //    contextMenu.addAction(actionRenameFeature);
    //    contextMenu.addAction(actionRemoveFeature);
    //    contextMenu.addAction(actionRemoveFeatures);
        contextMenu.addAction(actionRemoveNode);
    //}
    //if(mVertexSelected || mEdgeSelected)
    //{
    //    contextMenu.addSeparator();
    //}
    //contextMenu.addAction(actionAddNode);
    //contextMenu.addSeparator();
    //contextMenu.addAction(actionImport);
    //contextMenu.addAction(actionExport);
    //contextMenu.addSeparator();
    //contextMenu.addAction(actionRefresh);
    //contextMenu.addAction(actionShuffle);
    //if(!mpGraph->empty())
    //{
    //    contextMenu.addAction(actionReset);
    //}
    //contextMenu.addAction(actionLayout);
    //contextMenu.addSeparator();

    //if(!WidgetManager::getInstance()->getPropertyDialog()->isRunning())
    //{
    //    contextMenu.addSeparator();
    //    contextMenu.addAction(actionReloadPropertyDialog);
    //}
    contextMenu.exec(mapToGlobal(pos));
}

void ComponentEditorWidget::addFeatureDialog()
{
    if(!mFocusedNodeItem)
    {
        LOG_WARN_S << "No current node to add feature on";
        updateStatus("No current node to add feature on", GraphWidgetManager::TIMEOUT);
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

    AddGraphElement graphElementDialog(supportedTypes, this);
    graphElementDialog.setWindowTitle("Add feature");
    graphElementDialog.exec();
    if(graphElementDialog.result() == QDialog::Accepted)
    {

        Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(graphElementDialog.getType().toStdString(),
                graphElementDialog.getLabel().toStdString());

        Edge::Ptr edge(new Edge(mFocusedNodeItem->getVertex(), vertex));
        graph()->addEdge(edge);
        updateStatus("Added feature '" + vertex->toString() + "' of type '" + vertex->getClassName() + "'", GraphWidgetManager::TIMEOUT);
        refresh();
    } else {
        updateStatus("Adding feature aborted by user", GraphWidgetManager::TIMEOUT);
    }
}

void ComponentEditorWidget::renameFeatureFocused()
{
    LOG_WARN_S << "Rename feature focused";
    //renameFeature(mpFocusedVertex);
}

void ComponentEditorWidget::renameFeatureSelected()
{
    LOG_WARN_S << "Rename feature selected";
    //renameFeature( getSelectedGra);
}

//void ComponentEditorWidget::renameFeature(graph_analysis::Vertex::Ptr concernedVertex)
//{
//    updateStatus(std::string("renaming a feature in vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "'...");
//    NodeItem *item = mNodeItemMap[concernedVertex];
//    if(!item)
//    {
//        std::string error_msg = std::string("graph_analysis::ComponentEditorWidget::renameFeature: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
//        LOG_ERROR_S << error_msg;
//        throw std::runtime_error(error_msg);
//    }
//    int featureCount = item->getFeatureCount();
//    if(!featureCount)
//    {
//        QMessageBox::critical(this, tr("Cannot Rename a Feature"), tr("The selected vertex had no features!"));
//        updateStatus(std::string("Failed to rename a feature of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + ": there are no features!", GraphWidgetManager::TIMEOUT);
//        return;
//    }
//    RenameFeatureDialog dialog(item);
//    if(dialog.isValid())
//    {
//        std::string newLabel    = dialog.getNewLabel();
//        std::string strFeatureID   = dialog.getFeatureID();
//        int featureID;
//        std::stringstream ss(strFeatureID);
//        ss >> featureID;
//        // having identified the feature to be renamed, ordering its re-labeling
//        item->setFeatureLabel(featureID, newLabel);
//        // does not forget to refresh the parallel read-only view of this base graph mpGraph (the one in the layers graph widget)
//        updateStatus(std::string("Renamed the feature of local ID '" + boost::lexical_cast<std::string>(featureID) + "' of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "' to '" + newLabel + "'!", GraphWidgetManager::TIMEOUT);
//    }
//    else
//    {
//        updateStatus(std::string("Failed to rename a feature of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + ": aborted by user!", GraphWidgetManager::TIMEOUT);
//    }
//}

void ComponentEditorWidget::removeFeatureFocused()
{
    LOG_WARN_S << "REMOVE FEATURE FOCUSED";
    //removeFeature(mpFocusedVertex);
}

void ComponentEditorWidget::removeFeatureSelected()
{
    LOG_WARN_S << "REMOVE FEATURE SELECTED";
    //removeFeature(mpSelectedVertex);
}

void ComponentEditorWidget::removeFeature(graph_analysis::Vertex::Ptr concernedVertex)
{
//    updateStatus(std::string("removing a feature from vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "'...");
    NodeItem *item = mNodeItemMap[concernedVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::ComponentEditorWidget::removeFeature: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        updateStatus(std::string("Failed to remove a feature of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + ": " + error_msg + "!", GraphWidgetManager::TIMEOUT);
        throw std::runtime_error(error_msg);
    }
    int featureCount = item->getFeatureCount();
    if(!featureCount)
    {
        QMessageBox::critical(this, tr("Cannot Remove a Feature"), tr("The selected vertex had no features!"));
        updateStatus(std::string("Failed to remove a feature of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + ": there are no features!", GraphWidgetManager::TIMEOUT);
        return;
    }

//    bool ok;
//    QStringList features_options;
//    foreach(NodeItem::VTuple tuple, item->getVertices())
//    {
//        graph_analysis::Vertex::Ptr vertex = tuple.second;
//        std::string option = boost::lexical_cast<std::string>(tuple.first) + ": " + vertex->getLabel();
//        features_options << tr(option.c_str());
//    }
//    QString strFeatureID = QInputDialog::getItem(this, tr("Remove a Feature"),
//                                         tr("Feature ID:"), features_options,
//                                         0, false, &ok);
//    if (ok && !strFeatureID.isEmpty())
//    {
//        std::stringstream ss(strFeatureID.toStdString());
//        int featureID;
//        ss >> featureID;
//        // remove conceptual edges
//        EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(item->getFeature(featureID));
//        while(edgeIt->next())
//        {
//            Edge::Ptr edge = edgeIt->current();
//            mpGraph->removeEdge(edge);
//        }
//        // remove physical edges and their graphics
//        graph_analysis::Vertex::Ptr cluster = item->getVertex();
//        edgeIt = mpLayoutingGraph->getEdgeIterator(cluster);
//        while(edgeIt->next())
//        {
//            Edge::Ptr edge = edgeIt->current();
//            if(mEdgeItemMap.count(edge))
//            {
//                EdgeItem *edgeItem = mEdgeItemMap[edge];
//                if  (
//                        edgeItem
//                            &&
//                        (
//                            (item == edgeItem->sourceNodeItem() && featureID == edgeItem->getSourcePortID()) ||
//                            (item == edgeItem->targetNodeItem() && featureID == edgeItem->getTargetPortID())
//                        )
//                    )
//                    {
//                        mpLayoutingGraph->removeEdge(edge);
//                        syncEdgeItemMap(edge);
//                        scene()->removeItem(edgeItem);
//                    }
//            }
//        }
//        // remove feature graphics
//        item->removeFeature(featureID);
//        updateStatus(std::string("Removed the feature of local ID '" + boost::lexical_cast<std::string>(featureID) + "' of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "'!", GraphWidgetManager::TIMEOUT);
//    }
//    else
//    {
//        updateStatus(std::string("Failed to remove a feature of vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + ": aborted by user!", GraphWidgetManager::TIMEOUT);
//    }
}

Edge::Ptr ComponentEditorWidget::createEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label)
{
    Edge::Ptr edge(new Edge(sourceNode, targetNode, label));
    // registering new edge
    mpGraph->addEdge(edge);
    enableEdge(edge);
    return edge;
}

Vertex::Ptr ComponentEditorWidget::createVertex(const std::string& type, const std::string& label)
{
    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
    std::set<std::string>::iterator type_it = types.find(type);
    if(types.end() == type_it)
    {
        std::string error_msg = std::string("graph_analysis::ComponentEditorWidget::createVertex: Given Vertex Type '") + type + "' is not registered!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    graph_analysis::Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(type, label);
    // registering new vertex
    mpGraph->addVertex(vertex);
    enableVertex(vertex);
    return vertex;
}

Edge::Ptr ComponentEditorWidget::createStandaloneEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label)
{
    Edge::Ptr edge(new Edge(sourceNode, targetNode, label));
    return edge; // without registration!
}

Vertex::Ptr ComponentEditorWidget::createStandaloneVertex(const std::string& type, const std::string& label)
{
    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
    std::set<std::string>::iterator type_it = types.find(type);
    if(types.end() == type_it)
    {
        std::string error_msg = std::string("graph_analysis::ComponentEditorWidget::createVertex: Given Vertex Type '") + type + "' is not registered!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    graph_analysis::Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(type, label);
    return vertex; // without registration!
}

void ComponentEditorWidget::removeFocusedEdge()
{
    LOG_WARN_S << "REMOVE FOCUSED EDGE";
    clearEdge(mpFocusedEdge);
    clearEdgeFocus();
}

void ComponentEditorWidget::removeSelectedEdge()
{
    LOG_WARN_S << "REMOVE EDGE";
    //clearEdge(mpSelectedEdge);
}

void ComponentEditorWidget::clearEdge(graph_analysis::Edge::Ptr concernedEdge)
{
    updateStatus("Removed edge '");
}

void ComponentEditorWidget::removeFocusedVertex()
{
    mFocusedNodeItem->removeFeatures();
    graph()->removeVertex( mFocusedNodeItem->getVertex() );
    refresh();
}

void ComponentEditorWidget::clearVertex(graph_analysis::Vertex::Ptr concernedVertex)
{
    updateStatus("Removed node '");
}

void ComponentEditorWidget::spawnEdge(const std::string& label) // assumes the concerned edge-creation member fields are properly set already
{
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

void ComponentEditorWidget::removeVertex(Vertex::Ptr vertex)
{
    mpGraph->removeVertex(vertex);
}

void ComponentEditorWidget::removeEdge(Edge::Ptr edge)
{
    mpGraph->removeEdge(edge);
}

void ComponentEditorWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
}

void ComponentEditorWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        // Use ScrollHand Drag Mode to enable Panning
        setDragMode(ScrollHandDrag);
        // deflecting the current event into propagating a custom default-panning left-mouse-button oriented behaviour
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        GraphWidget::mousePressEvent(&fake); // initiates scroll-button panning
    } else {
        GraphWidget::mousePressEvent(event);
    }
}

void ComponentEditorWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        // Use ScrollHand Drag Mode to desable Panning
        setDragMode(NoDrag);
        // deflecting the current event into propagating a custom default-panning left-mouse-button oriented behaviour
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mouseReleaseEvent(&fake); // terminates scroll-button panning
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void ComponentEditorWidget::updateMoveAround(bool moveAround)
{
    WidgetManager::getInstance()->getPropertyDialog()->uncheckDragDrop(moveAround);
    setDragDrop(!moveAround);
}

void ComponentEditorWidget::updateDragDrop(bool dragDrop)
{
    WidgetManager::getInstance()->getPropertyDialog()->uncheckMoveAround(dragDrop);
    setDragDrop(dragDrop);
}

void ComponentEditorWidget::setDragDrop(bool dragDrop)
{
    updateStatus(std::string("toggling drag-n-drop mode to ") + (dragDrop ? "true" : "false") + "...");
    mDragDrop = dragDrop;
    NodeItemMap::iterator it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != it; ++it)
    {
        NodeItem *current = it->second;
        if(graph_analysis::ClusterVertex::vertexType() == current->getVertex()->getClassName())
        {
            current->setHandlesChildEvents(!mDragDrop);
            current->setFlag(QGraphicsItem::ItemIsMovable, !mDragDrop);
        }
    }
    updateStatus(std::string("Toggled drag-n-drop mode to ") + (dragDrop ? "true" : "false") + "...", GraphWidgetManager::TIMEOUT);
}

void ComponentEditorWidget::syncDragDrop()
{
    NodeItemMap::iterator it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != it; ++it)
    {
        NodeItem *current = it->second;
        if(graph_analysis::ClusterVertex::vertexType() == current->getVertex()->getClassName())
        {
            current->setHandlesChildEvents(!mDragDrop);
            current->setFlag(QGraphicsItem::ItemIsMovable, !mDragDrop);
        }
    }
}

void ComponentEditorWidget::setDragDrop()
{}

void ComponentEditorWidget::unsetDragDrop()
{}

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
    updateStatus(status_msg, GraphWidgetManager::TIMEOUT);
}

void ComponentEditorWidget::shuffle()
{
    updateStatus("Shuffelling all nodes ...");
    foreach (QGraphicsItem *item, scene()->items())
    {
        if (qgraphicsitem_cast<NodeItem* >(item))
        {
            item->setPos(-150 * mScaleFactor + mScaleFactor * (qrand() % 300), -150 * mScaleFactor + mScaleFactor * (qrand() % 300));
        }
    }
    updateStatus("Done shuffelling all nodes", GraphWidgetManager::TIMEOUT);
}

void ComponentEditorWidget::zoomIn()
{
    scaleView(qreal(1.13));
}

void ComponentEditorWidget::zoomOut()
{
    scaleView(1 / qreal(1.13));
}

void ComponentEditorWidget::setVertexFocused(bool focused)
{
    mVertexFocused = focused;
    WidgetManager::getInstance()->getPropertyDialog()->setVertexFocused(focused);
}

void ComponentEditorWidget::setEdgeFocused(bool focused)
{
    mEdgeFocused = focused;
    WidgetManager::getInstance()->getPropertyDialog()->setEdgeFocused(focused);
}

void ComponentEditorWidget::clearNodeFocus()
{
    if(mVertexFocused)
    {
        NodeItem *item = mNodeItemMap[mpFocusedVertex];
        item->releaseFocus();
        updateStatus(std::string("Cleared node focus off node '") + mpFocusedVertex->toString() + "'!", GraphWidgetManager::TIMEOUT);
    }
}

void ComponentEditorWidget::clearEdgeFocus()
{
    if(mEdgeFocused)
    {
        EdgeItem *item = mEdgeItemMap[mpFocusedEdge];
        item->releaseFocus();
        updateStatus(std::string("Cleared edge focus off edge '") + mpFocusedEdge->toString() + "'!", GraphWidgetManager::TIMEOUT);
    }
}

void ComponentEditorWidget::clearFocus()
{
    clearNodeFocus();
    clearEdgeFocus();
    if(mVertexFocused && mpFocusedEdge)
    {
        updateStatus(std::string("Cleared focuses: ")
                        + "off node '" + mpFocusedVertex->toString()
                        + "' and off edge '" + mpFocusedEdge->toString() + "'!"
                        , GraphWidgetManager::TIMEOUT
                    );
    }
}


void ComponentEditorWidget::removeFeaturesFocused()
{
    LOG_WARN_S << "Remove features focused";
    //removeFeatures(mpFocusedVertex);
}

void ComponentEditorWidget::removeFeaturesSelected()
{
    LOG_WARN_S << "Remove features selected";
    //removeFeatures(mpSelectedVertex);
}

void ComponentEditorWidget::removeFeatures(graph_analysis::Vertex::Ptr concernedVertex)
{
    updateStatus(std::string("Removing all features from vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "'...");
    NodeItem *item = mNodeItemMap[concernedVertex];
    // error checking on features removal
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::ComponentEditorWidget::removeFeatures: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        updateStatus(std::string("Failed to remove all features from vertex '") + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "': " + error_msg + "!", GraphWidgetManager::TIMEOUT);
        throw std::runtime_error(error_msg);
    }

    int nfeatures = item->getFeatureCount();
    if(!nfeatures)
    {
        QMessageBox::critical(this, tr("No features to remove"), "The cluster is already empty!");
        updateStatus("Failed to remove all features from vertex '" + concernedVertex->toString() + "' of type '" + concernedVertex->getClassName() + "': there are no features", GraphWidgetManager::TIMEOUT);
        return;
    } else
    {
        // prompting the user for all features deletion
        QMessageBox::StandardButton button = QMessageBox::question(this, tr("Confirm complete features-removal"), tr((QString("All features in node '") + QString(concernedVertex->getLabel().c_str()) + QString("' will be deleted! Are you sure you want to continue?")).toAscii()), QMessageBox::Yes | QMessageBox::No);

        graph_analysis::Vertex::Ptr cluster = item->getVertex();
        EdgeIterator::Ptr edgeIt;

        if(button == QMessageBox::Yes)
        {
            item->removeFeatures();
            //// remove physical edges that are connected with this feature
            //foreach(items::Feature* feature, item->getFeatures())
            //{
            //    edgeIt = mpGraph->getEdgeIterator(feature->getVertex());
            //    while(edgeIt->next())
            //    {
            //        Edge::Ptr edge = edgeIt->current();
            //        mpGraph->removeEdge(edge);
            //    }
            //}

            refresh();
        }
    }
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

        if(graph_analysis::ClusterVertex::vertexType() == vertex->getClassName())
        {
            // Registering new Cluster node items only
            NodeItem* nodeItem = NodeTypeManager::getInstance()->createItem(this, vertex);
            mNodeItemMap[vertex] = nodeItem;
            scene()->addItem(nodeItem);
            mpLayoutingGraph->addVertex(vertex);
            mpGVGraph->addNode(vertex);

            LOG_DEBUG_S << "Adding vertex of type: " << vertex->getClassName();
        } else {
            LOG_DEBUG_S << "Not adding vertex of type: " << vertex->getClassName();

        }
    }

    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        // Check on active filter
        if(mFiltered && !mpSubGraph->enabled(edge))
        {
            LOG_DEBUG_S << "Filtered out an edge of filtering value: " << mpSubGraph->enabled(edge);
            continue;
        }

        if(mEdgeItemMap.count(edge))
        {
            continue;
        }

        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        std::string sourceClassName = source->getClassName();
        std::string targetClassName = target->getClassName();

        if(graph_analysis::OutputPortVertex::vertexType() == sourceClassName && graph_analysis::InputPortVertex::vertexType() == targetClassName)
        {
            // physical edge - processing deflected until after all features will have been registered
            continue;
        }
//        else if (   // disabled Port vertices (only specialized InputPorts/OutputPortsVertex allowed
//                    (graph_analysis::PortVertex::vertexType() == sourceClassName && graph_analysis::ClusterVertex::vertexType() == targetClassName)
//                )
//        {
//            // semantical edge: links a cluster vertex to one of its features
//            std::string warn_msg = std::string("graph_analysis::ComponentEditorWidget::updateLayout: found reversed edge from source feature vertex '") +
//                                        source->toString() + "' of type '" + sourceClassName + "' to target Cluster vertex '" +
//                                        target->toString() + "' of type '" + targetClassName + "'!";
//            LOG_WARN_S << warn_msg; // warn. due to cluster being set as target of the semantically valid edge
//            NodeItem* targetNodeItem = mNodeItemMap[ target ];
//            if(!targetNodeItem)
//            {
//                continue;
//            }
//            mFeatureMap[source] = targetNodeItem;
//            mFeatureIDMap[source] = targetNodeItem->addFeature(source);
//        }
        else if (
                    (
                        graph_analysis::ClusterVertex::vertexType() == sourceClassName
                            &&
                        (
                            graph_analysis::InputPortVertex::vertexType() == targetClassName
                                ||
                            graph_analysis::OutputPortVertex::vertexType() == targetClassName
                                ||
                            graph_analysis::PropertyVertex::vertexType() == targetClassName
                                ||
                            graph_analysis::OperationVertex::vertexType() == targetClassName
                        )
                    )
                )
        {
            // semantical edge: links a cluster vertex to one of its features
            NodeItem* sourceNodeItem = mNodeItemMap[ source ];
            if(!sourceNodeItem)
            {
                continue;
            }
            mFeatureMap[target] = sourceNodeItem;

            items::Feature* feature = new items::Feature(target, this);
            mFeatureIDMap[target] = sourceNodeItem->addFeature(feature);
        }
        else
        {
            //// invalid edge
            //std::string error_msg = std::string("graph_analysis::ComponentEditorWidget::updateLayout: found invalid edge from source vertex '") +
            //                            source->toString() + "' of type '" + sourceClassName + "' to target vertex '" +
            //                            target->toString() + "' of type '" + targetClassName + "'!";
            //LOG_ERROR_S << error_msg;
            //throw std::runtime_error(error_msg);
        }
    }

//    // re-iterating the edges for rendering the physical ones
//    edgeIt = mpGraph->getEdgeIterator();
//    while(edgeIt->next())
//    {
//        Edge::Ptr edge = edgeIt->current();
//
//        // Check on active filter
//        if(mFiltered && !mpSubGraph->enabled(edge))
//        {
//            LOG_DEBUG_S << "graph_analysis::gui::ComponentEditorWidget: Filtered out edge: " << edge->toString();
//            continue;
//        }
//
//        if(mEdgeItemMap.count(edge))
//        {
//            continue;
//        }
//
//        Vertex::Ptr source = edge->getSourceVertex();
//        Vertex::Ptr target = edge->getTargetVertex();
//
//        if(graph_analysis::OutputPortVertex::vertexType() == source->getClassName() && graph_analysis::InputPortVertex::vertexType() == target->getClassName())
//        {
//            NodeItem* sourceNodeItem = mFeatureMap[ source ];
//            NodeItem* targetNodeItem = mFeatureMap[ target ];
//            // physical edge - processing was deflected until now - i.e. after all features will have been registered
//            Edge::Ptr default_edge(new Edge(sourceNodeItem->getVertex(), targetNodeItem->getVertex(), edge->getLabel()));
//            mpLayoutingGraph->addEdge(default_edge);
//
//            //EdgeItem* edgeItem = EdgeTypeManager::getInstance()->createItem(this, sourceNodeItem, mFeatureIDMap[source], targetNodeItem, mFeatureIDMap[target], default_edge);
//            //scene()->addItem(edgeItem);
//            //mEdgeItemMap[default_edge] = edgeItem;
//            //mpGVGraph->addEdge(default_edge);
//            //mEdgeMap[default_edge] = edge;
//        }
//    }

    // computing max node height and width for informing GraphViz of max dimensions w.r.t. immediately subsequent layouting
    NodeItemMap::iterator node_it = mNodeItemMap.begin();
    for(; mNodeItemMap.end() != node_it; ++node_it)
    {
        QRectF nodeBoundingRect = node_it->second->boundingRect();
        qreal height = nodeBoundingRect.height();
        qreal width  = nodeBoundingRect.width();
        if(mMaxNodeHeight < height)
        {
            height = mMaxNodeHeight;
        }
        if(mMaxNodeWidth < width)
        {
            mMaxNodeWidth = width;
        }
    }

    // the mode is synchronized with the current state in the other layers (i.e. the mode that was active right before graphical graph reloading)
    //syncDragDrop();

}

void ComponentEditorWidget::swapFeaturesFocused()
{
    LOG_WARN_S << "Swap features focused";
    //swapFeatures(mpFocusedVertex);
}

void ComponentEditorWidget::swapFeaturesSelected()
{
    LOG_WARN_S << "Swap features selected";
    //swapFeatures(mpSelectedVertex);
}

void ComponentEditorWidget::swapFeatures(graph_analysis::Vertex::Ptr concernedVertex)
{
    updateStatus("swapping features within node '" + concernedVertex->toString() + "' ...");
    NodeItem *item = mNodeItemMap[concernedVertex];
    if(!item)
    {
        std::string error_msg = std::string("graph_analysis::ComponentEditorWidget::swapFeatures: provided vertex '") + concernedVertex->getLabel() + "' is not registered with the GUI";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    int featureCount = item->getFeatureCount();
    if(featureCount < 2)
    {
        QMessageBox::critical(this, tr("Cannot Swap Features"), tr("The selected vertex did not have enough features!"));
        updateStatus("Failed to swap features within node '" + concernedVertex->toString() + "': this vertex did not have enough features!", GraphWidgetManager::TIMEOUT);
        return;
    }
    SwapFeaturesDialog dialog(item);
    if(dialog.isValid())
    {
        std::string strFeature1ID   = dialog.getFeature1ID();
        std::string strFeature2ID   = dialog.getFeature2ID();
        int feature1ID;
        std::stringstream ss1(strFeature1ID);
        ss1 >> feature1ID;
        int feature2ID;
        std::stringstream ss2(strFeature2ID);
        ss2 >> feature2ID;
        if(feature1ID - feature2ID)
        {
            try
            {
                item->swapFeatures(feature1ID, feature2ID);
            }
            catch(std::runtime_error e)
            {
                LOG_ERROR_S << "graph_analysis::gui::ComponentEditorWidget::swapFeatures: swapping operation failed: " << e.what();
                QMessageBox::critical(this, tr("Swapping Failed"), QString(e.what()));
                updateStatus("Failed to swap features within node '" + concernedVertex->toString() + "': " + std::string(e.what()) + "!", GraphWidgetManager::TIMEOUT);
            }
        }
        else
        {
            QMessageBox::information(this, tr("Swapped Features In-place"), tr("identical features were selected!"));
        }
        updateStatus("Swapped features of IDs " + boost::lexical_cast<std::string>(feature1ID) + " and "
                        + boost::lexical_cast<std::string>(feature2ID) + " within node '" + concernedVertex->toString()
                        , GraphWidgetManager::TIMEOUT
                    );
    }
    else
    {
        updateStatus("Failed to swap features within node '" + concernedVertex->toString() + "': aborted by user!", GraphWidgetManager::TIMEOUT);
    }
}

} // end namespace gui
} // end namespace graph_analysis
