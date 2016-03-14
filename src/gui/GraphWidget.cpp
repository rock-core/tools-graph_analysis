#include "GraphWidget.hpp"

#include <set>
#include <math.h>
#include <sstream>
#include <vector>
#include <exception>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <base/Logging.hpp>
#include <base/Time.hpp>

#include <graph_analysis/Filter.hpp>
#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/io/YamlWriter.hpp>
#include <graph_analysis/io/GexfWriter.hpp>
#include <graph_analysis/io/GexfReader.hpp>
#include <graph_analysis/io/YamlReader.hpp>
#include <graph_analysis/io/GraphvizWriter.hpp>
#include <graph_analysis/filters/CommonFilters.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>
#include <graph_analysis/gui/items/EdgeLabel.hpp>
#include <graph_analysis/gui/WidgetManager.hpp>
#include <graph_analysis/gui/GraphWidgetManager.hpp>
#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <QDir>
#include <QTime>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSignalMapper>

#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "NodeItemTypeManager.hpp"
#include "EdgeItemTypeManager.hpp"
#include "ActionCommander.hpp"
#include "dialogs/AddGraphElement.hpp"
#include "dialogs/PropertyDialog.hpp"

#define DEFAULT_SCALING_FACTOR 2.269

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
    , mpGVGraph(NULL)
    , mMaxNodeHeight(0)
    , mMaxNodeWidth (0)
    , mFiltered(false)
    , mScaleFactor(DEFAULT_SCALING_FACTOR)
    , mLayout("dot")
    //, mpVertexFilter(new filters::PermitAll< graph_analysis::Vertex::Ptr>( ))
    , mpVertexFilter(new Filter< graph_analysis::Vertex::Ptr>( ))
    , mpEdgeFilter(new filters::EdgeContextFilter())
    , mpGraphWidgetManager(NULL)
    , mMode(GraphWidgetManager::MOVE_MODE)
{
    mGraphView.setVertexFilter(mpVertexFilter);
    mGraphView.setEdgeFilter(mpEdgeFilter);
}

GraphWidget::GraphWidget(QMainWindow *mainWindow, QWidget *parent)
    : QGraphicsView(parent)
    , mpGVGraph(NULL)
    , mMaxNodeHeight(0)
    , mMaxNodeWidth (0)
    , mFiltered(false)
    , mScaleFactor(DEFAULT_SCALING_FACTOR)
    , mLayout("dot")
    , mpVertexFilter(new Filter< graph_analysis::Vertex::Ptr>())
    , mpEdgeFilter(new filters::EdgeContextFilter())
    , mpGraphWidgetManager(NULL)
{
    mGraphView.setVertexFilter(mpVertexFilter);
    mGraphView.setEdgeFilter(mpEdgeFilter);
}

GraphWidget::~GraphWidget()
{
}

void GraphWidget::setNodeFilters(std::vector< Filter<Vertex::Ptr>::Ptr > filters)
{
    mpVertexFilter->clear();
    BOOST_FOREACH(Filter<Vertex::Ptr>::Ptr filter, filters)
    {
        mpVertexFilter->add(filter);
    }
    mGraphView.setVertexFilter(mpVertexFilter);
}

void GraphWidget::setEdgeFilters(std::vector< Filter<Edge::Ptr>::Ptr > filters)
{
    mpEdgeFilter->clear();
    BOOST_FOREACH(Filter<Edge::Ptr>::Ptr filter, filters)
    {
        mpEdgeFilter->add(filter);
    }
    mGraphView.setEdgeFilter(mpEdgeFilter);
}

void GraphWidget::updateStatus(const std::string& message, int timeout) const
{
    WidgetManager::getInstance()->getGraphWidgetManager()->updateStatus(QString(message.c_str()), timeout);
}

void GraphWidget::setGraphLayout(const QString& layoutName)
{
    mLayout = layoutName;
    refresh();
}

void GraphWidget::clearVisualization()
{
    // TO-DO: add filtering clearing?
    if(mpGVGraph)
    {
        mpGVGraph->clearEdges();
        mpGVGraph->clearNodes();
    }

    mNodeItemMap.clear();
    mEdgeItemMap.clear();
    scene()->clear();
}

void GraphWidget::reset(bool keepData)
{
    getGraphWidgetManager()->resetGraph(keepData);
}

void GraphWidget::clearDialog()
{
    updateStatus(std::string("Clear the graph ..."));
    if(mpGraph->empty())
    {
        QMessageBox::information(this, tr("Nothing to clear"), "The graph is empty");
        updateStatus("Done clearing the graph");
    }
    else
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, tr("Confirm clear"), tr("The graph will be erased! Are you sure you want to continue?"), QMessageBox::Yes | QMessageBox::No);
        switch(button)
        {
            case QMessageBox::Yes:
                reset(false);
                updateStatus("Done clearing the graph");
            break;

            default:
                updateStatus("Clearing the graph aborted by user");
            break;
        }
    }
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
    updateFilterView();
    updateLayoutView();
}

void GraphWidget::updateFilterView()
{
    mpSubGraph = mGraphView.apply(mpGraph);
    mFiltered = true;
}

void GraphWidget::updateLayoutView()
{
    resetLayoutingGraph();

    // implemented by GraphWidgets
    // needs to populate the layouting graph as needed
    updateLayout();

    // apply layouting - i.e. loading the designated layouting base graph into GraphViz then repositioning the correspoding scene nodes
    if(mLayout.toLower() != "force")
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        LOG_INFO_S << "GV started layouting the graph. This can take a while ...";
        base::Time start = base::Time::now();
        mpGVGraph->setNodeAttribute("height", boost::lexical_cast<std::string>(mMaxNodeHeight));
        mpGVGraph->setNodeAttribute("width" , boost::lexical_cast<std::string>(mMaxNodeWidth ));
        LOG_INFO_S << "Applying layout: " << mLayout.toStdString();
        mpGVGraph->applyLayout(mLayout.toStdString());
        base::Time delay = base::Time::now() - start;
        QApplication::restoreOverrideCursor();
        LOG_INFO_S << "GV layouted the graph after " << delay.toMilliseconds() << "ms";
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
                // repositioning node in a scaled fashion
                nodeItem->setPos(mScaleFactor * gvNode.x(), mScaleFactor * gvNode.y());
            }
        }
    }
}

void GraphWidget::refresh(bool all)
{
    LOG_DEBUG_S << "Refresh widget: " << getClassName().toStdString() << " keep all data";
    reset(true /*keepData*/);

    update();

    // Making sure mode applies to all elements
    modeChanged(mMode);
}

QStringList GraphWidget::getSupportedLayouts() const
{
    QStringList options;

    std::set<std::string> supportedLayouts = mpGVGraph->getSupportedLayouts();
    foreach(std::string supportedLayout, supportedLayouts)
    {
        options << tr(supportedLayout.c_str());
    }

    return options;
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
    // some error checking
    if(!mpGraphWidgetManager)
    {
        throw std::runtime_error("graph_analysis::gui::GraphWidget::"
                                 "keyPressEvent: GraphWidgetManager is not set "
                                 "for this widget");
    }

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
            getGraphWidgetManager()->resetGraph();
            return;
        }

        case Qt::Key_E:
        case Qt::Key_S: {
            // CTRL+S (save) or CTRL+E (export graph) saves the graph to file
            getGraphWidgetManager()->exportGraph();
            return;
        }

        case Qt::Key_O:
        case Qt::Key_I:
        {
            // CTRL+O (open) or CTRL+I (input graph)
            getGraphWidgetManager()->importGraph();
            return;
        }
        case Qt::Key_L:
        {
            // CTRL+L (layout graph) opens a graph from file
            getGraphWidgetManager()->selectLayout();
            return;
        }

        case Qt::Key_P:
        {
            // CTRL+P reloads the property dialog (if it is currently not
            // running)
            if(!WidgetManager::getInstance()->getPropertyDialog()->isRunning())
            {
                WidgetManager::getInstance()
                    ->getGraphWidgetManager()
                    ->reloadPropertyDialog();
                return;
            }
        }
        }
    }
    // if we did not act, we shall call the handler of the base-class to
    // propagate the event.
    QGraphicsView::keyPressEvent(event);
}

GraphWidgetManager* GraphWidget::getGraphWidgetManager() const
{
    if(!mpGraphWidgetManager)
    {
        throw std::runtime_error("graph_analysis::gui::GraphWidget::getGraphWigetManager: widget manager is NULL");
    }
    return mpGraphWidgetManager;
}

void GraphWidget::selectElement(const graph_analysis::GraphElement::Ptr& element)
{
    std::vector<GraphElement::Ptr>::const_iterator cit = std::find(mElementSelection.begin(),
            mElementSelection.end(), element);

    if(cit == mElementSelection.end())
    {
        LOG_DEBUG_S << "Select element: '" << element->toString();
        mElementSelection.push_back(element);
    } else {
        throw std::invalid_argument("graph_analysis::gui::GraphWidget::selectElement: '" + element->toString() + "' already in selection");
    }
}

void GraphWidget::unselectElement(const graph_analysis::GraphElement::Ptr& element)
{
    // no const interator, will be used to erease later
    std::vector<GraphElement::Ptr>::iterator it =
        std::find(mElementSelection.begin(), mElementSelection.end(), element);

    if(it == mElementSelection.end())
    {
        throw std::invalid_argument("graph_analysis::gui::GraphWidget::unselectElement: '" + element->toString() + "' not in selection");
    } else {
        LOG_DEBUG_S << "Unselect element: '" << element->toString();
        mElementSelection.erase(it);
    }
}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        GraphElement::Ptr element = getFocusedElement();
        if(!element)
        {
            clearElementSelection();
        } else {
            // Allow to use SHIFT to create selection group
            if(event->modifiers() != Qt::ShiftModifier)
            {
                clearElementSelection();
            }

            try {
                selectElement(element);
            } catch(const std::runtime_error& e)
            {
                unselectElement(element);
            }

            if(getMode() == GraphWidgetManager::CONNECT_MODE)
            {
                QDrag* drag = new QDrag(this);
                QMimeData* mimeData = new QMimeData;

                // Setting the id of the graph element as mime data
                std::stringstream ss;
                ss << element->getId(mpGraph->getId());
                mimeData->setText(ss.str().c_str());
                drag->setMimeData(mimeData);

                drag->exec(Qt::CopyAction); // | Qt::MoveAction);
            }
        }

        QGraphicsView::mousePressEvent(event);
    } else if(event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to enable Panning
        setDragMode(ScrollHandDrag);
        // deflecting the current event into propagating a custom default-panning left-mouse-button oriented behaviour
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mousePressEvent(&fake);
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::MidButton)
    {
        //Use ScrollHand Drag Mode to end Panning
        setDragMode(NoDrag);
        QMouseEvent fake(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mouseReleaseEvent(&fake);
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void GraphWidget::renameElement(GraphElement::Ptr element, const std::string& label)
{
    element->setLabel(label);
}

// EDIT
Vertex::Ptr GraphWidget::addVertex(const std::string& type, const std::string& label, QPoint* position)
{
    LOG_DEBUG_S << "Add vertex: type: " << type << " label: " << label;
    graph_analysis::Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(type, label);
    mpGraph->addVertex(vertex);
    return vertex;
}

void GraphWidget::addVertexDialog(QObject* object)
{
    LOG_DEBUG_S << "Add vertex dialog";
    updateStatus("Adding vertex ...");

    // the scene position where to place the new node
    QPoint *position = (QPoint *) object;
    position = new QPoint(100,100);

    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
    QStringList supportedTypes;
    std::set<std::string>::const_iterator cit = types.begin();
    for(; cit != types.end(); ++cit)
    {
        supportedTypes << QString(cit->c_str());
    }

    dialogs::AddGraphElement graphElementDialog(supportedTypes, this);
    graphElementDialog.exec();
    if(graphElementDialog.result() == QDialog::Accepted)
    {
        Vertex::Ptr vertex = addVertex(graphElementDialog.getType().toStdString(), graphElementDialog.getLabel().toStdString(), position);
        updateStatus("Added vertex '" + vertex->toString() + "' of type '" + vertex->getClassName() + "'", GraphWidgetManager::TIMEOUT);
        refresh();
    } else
    {
        updateStatus("Adding vertex aborted by user", GraphWidgetManager::TIMEOUT);
    }
}

Edge::Ptr GraphWidget::addEdge(const std::string& type, const std::string& label, Vertex::Ptr sourceVertex, Vertex::Ptr targetVertex)
{
    Edge::Ptr edge = EdgeTypeManager::getInstance()->createEdge(type, sourceVertex, targetVertex, label);
    mpGraph->addEdge(edge);
    return edge;
}

void GraphWidget::addEdgeDialog(Vertex::Ptr sourceVertex, Vertex::Ptr targetVertex)
{
    updateStatus("Adding edge ...");
    std::set<std::string> types = EdgeTypeManager::getInstance()->getSupportedTypes();
    std::set<std::string>::const_iterator cit = types.begin();
    QStringList typesList;
    for(; cit != types.end(); ++cit)
    {
        typesList << cit->c_str();
    }

    dialogs::AddGraphElement graphElementDialog(typesList);

    LOG_DEBUG_S << "Add edge dialog";

    graphElementDialog.exec();
    if(graphElementDialog.result() == QDialog::Accepted)
    {
        Edge::Ptr edge = addEdge(graphElementDialog.getType().toStdString(), graphElementDialog.getLabel().toStdString(), sourceVertex, targetVertex);
        updateStatus("Added edge '" + edge->toString() + "' of type '" + edge->getClassName() + "' with label: " + graphElementDialog.getLabel().toStdString(),
                GraphWidgetManager::TIMEOUT);
        refresh();
    } else
    {
        updateStatus("Adding edge aborted by user", GraphWidgetManager::TIMEOUT);
    }
}

NodeItem* GraphWidget::getFocusedNodeItem() const
{
    NodeItemMap::const_iterator cit = mNodeItemMap.begin();
    for(; cit != mNodeItemMap.end(); ++cit)
    {
        NodeItem* nodeItem = cit->second;
        if(nodeItem->isUnderMouse())
        {
            return nodeItem;
        }
    }
    return NULL;
}

EdgeItem* GraphWidget::getFocusedEdgeItem() const
{
    EdgeItemMap::const_iterator cit = mEdgeItemMap.begin();
    for(; cit != mEdgeItemMap.end(); ++cit)
    {
        EdgeItem* edgeItem = cit->second;
        if(edgeItem->isUnderMouse())
        {
            return edgeItem;
        }
    }
    return NULL;
}

void GraphWidget::selectLayoutDialog()
{
    updateStatus(std::string("Selecting graph layout..."));

    bool ok;
    QString desiredLayout = QInputDialog::getItem(this, tr("Select Layout"),
                                         tr("select a layout:"), getSupportedLayouts(),
                                         0, false, &ok);
    if (ok && !desiredLayout.isEmpty())
    {
        refresh(true /*keep all data*/);
        setGraphLayout(desiredLayout);

        updateStatus("Changed graph layout to '" + desiredLayout.toStdString() + "'");
    } else
    {
        updateStatus("Changing graph layout aborted by user");
    }
}

void GraphWidget::editElementDialog(const GraphElement::Ptr& element)
{
    GraphElement::Ptr elementToEdit;
    if(element)
    {
        elementToEdit = element;
    } else if( !(elementToEdit = getFocusedElement()) )
    {
        updateStatus("Editing failed -- cannot identify element which shall be edited");
        return;
    }

    updateStatus("Editing '" + elementToEdit->toString() + "'");
    bool ok;
    QString label = QInputDialog::getText(this, tr(std::string("Change label of '" + elementToEdit->toString() + "'").c_str()),
                                         tr("New label:"), QLineEdit::Normal,
                                         QString(elementToEdit->getLabel().c_str()), &ok);
    if (ok && !label.isEmpty())
    {
        std::string old_label = elementToEdit->toString();
        std::string new_label = label.toStdString();

        renameElement(elementToEdit, new_label);
        updateStatus("Changed from '" + old_label + "' to '" + new_label + "'");
        refresh();
    } else
    {
        updateStatus("Editing aborted by user");
    }
}

void GraphWidget::removeElement(const GraphElement::Ptr& element)
{
    if(element)
    {
        Vertex::Ptr vertex = dynamic_pointer_cast<Vertex>(element);
        if(vertex)
        {
            removeVertex(vertex);
            return;
        }
        Edge::Ptr edge = dynamic_pointer_cast<Edge>(element);
        if(edge)
        {
            removeEdge(edge);
            return;
        }
    }
}
void GraphWidget::removeVertex(const Vertex::Ptr& vertex)
{
    // Check if vertex relates to a registered node item
    NodeItemMap::const_iterator it = mNodeItemMap.find(vertex);
    if(it == mNodeItemMap.end())
    {
        // not a node item (but a feature) -- so safe to remove edge
    } else {
        NodeItem* nodeItem = it->second;
        nodeItem->removeFeatures();
    }
    mpGraph->removeVertex(vertex);
}

void GraphWidget::removeEdge(const Edge::Ptr& edge)
{
    mpGraph->removeEdge(edge);
}

void GraphWidget::removeElementDialog(const GraphElement::Ptr& element)
{
    GraphElement::Ptr elementToRemove;
    if(element)
    {
        elementToRemove = element;
    } else if( !(elementToRemove = getFocusedElement()) )
    {
        updateStatus("Removal failed -- cannot identify element which shall be removed");
        return;
    }

    updateStatus("Removing '" + elementToRemove->toString() + "'");
    QMessageBox msgBox;
    msgBox.setText(tr(std::string("Remove '" + elementToRemove->toString() + "'?").c_str()) );
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    if(QMessageBox::Ok == msgBox.exec())
    {
        removeElement(elementToRemove);
        refresh();
    } else
    {
        updateStatus("Removal aborted by user");
    }
}

void GraphWidget::modeChanged(GraphWidgetManager::Mode mode)
{
    switch(mode)
    {
        case GraphWidgetManager::CONNECT_MODE:
            LOG_DEBUG_S << "Mode changed to connect mode";
            setFlagOnAllItems(QGraphicsItem::ItemIsMovable, false);
            break;
        case GraphWidgetManager::EDIT_MODE:
            LOG_DEBUG_S << "Mode changed to edit mode";
            setFlagOnAllItems(QGraphicsItem::ItemIsMovable, false);
            break;
        case GraphWidgetManager::MOVE_MODE:
            LOG_DEBUG_S << "Mode changed to move mode";
            setFlagOnAllItems(QGraphicsItem::ItemIsMovable, true);
            break;
        default:
            throw std::invalid_argument("graph_analysis::gui::GraphWidget: "
                    "changed into unknown mode");
    }

    mMode = mode;
}

void GraphWidget::setFlagOnAllItems(QGraphicsItem::GraphicsItemFlag flag, bool value)
{
    {
        NodeItemMap::iterator it = mNodeItemMap.begin();
        for(; it != mNodeItemMap.end(); ++it)
        {
            NodeItem* nodeItem = it->second;
            nodeItem->setFlag(flag, value);
        }
    }

    {
        EdgeItemMap::iterator it = mEdgeItemMap.begin();
        for(; it != mEdgeItemMap.end(); ++it)
        {
            EdgeItem* edgeItem = it->second;
            edgeItem->setFlag(flag, value);
        }
    }
}


} // end namespace gui
} // end namespace graph_analysis
