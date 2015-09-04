#include "GraphWidget.hpp"
#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "AddGraphElement.hpp"
#include "PropertyDialog.hpp"
#include "NodeTypeManager.hpp"
#include "EdgeTypeManager.hpp"
#include "ActionCommander.hpp"
#include "SwapFeaturesDialog.hpp"
#include "RenameFeatureDialog.hpp"

#include <set>
#include <math.h>
#include <sstream>
#include <QDir>
#include <QTime>
#include <QMenu>
#include <sstream>
#include <QMenuBar>
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

#include <exception>
#include <boost/foreach.hpp>
#include <base/Time.hpp>
#define DEFAULT_SCALING_FACTOR 2.269

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

GraphWidget::GraphWidget(const QString& widgetName, QWidget *parent)
    : QGraphicsView(parent)
    , mWidgetName(widgetName)
    , mpGraph()
    , mpGVGraph(0)
    , mpLayoutingGraph()
    , mMaxNodeHeight(0)
    , mMaxNodeWidth (0)
    , mFiltered(false)
    , mTimerId(0)
    , mScaleFactor(DEFAULT_SCALING_FACTOR)
    , mLayout("dot")
    //, mpVertexFilter(new filters::PermitAll< graph_analysis::Vertex::Ptr>( ))
    , mpVertexFilter(new Filter< graph_analysis::Vertex::Ptr>( ))
    , mpEdgeFilter(new filters::EdgeContextFilter())
    , mpGraphWidgetManager(0)
{
    mGraphView.setVertexFilter(mpVertexFilter);
    mGraphView.setEdgeFilter(mpEdgeFilter);
}

GraphWidget::GraphWidget(QMainWindow *mainWindow, QWidget *parent)
    : QGraphicsView(parent)
    , mpGraph()
    , mpGVGraph(0)
    , mpLayoutingGraph()
    , mMaxNodeHeight(0)
    , mMaxNodeWidth (0)
    , mFiltered(false)
    , mTimerId(0)
    , mScaleFactor(DEFAULT_SCALING_FACTOR)
    , mLayout("dot")
    , mpVertexFilter(new Filter< graph_analysis::Vertex::Ptr>())
    , mpEdgeFilter(new filters::EdgeContextFilter())
    , mpGraphWidgetManager(0)
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

void GraphWidget::clear()
{
    // TO-DO: add filtering clearing?
    if(mpGVGraph)
    {
        mpGVGraph->clearEdges();
        mpGVGraph->clearNodes();
    }

    mNodeItemMap.clear();
    mEdgeItemMap.clear();
    mFeatureIDMap.clear();
    mFeatureMap.clear();
    mEdgeMap.clear();
    scene()->clear();
}

void GraphWidget::reset(bool keepData)
{
    getGraphWidgetManager()->resetGraph(keepData);
}

void GraphWidget::clearWithDialog()
{
    updateStatus(std::string("Clear the graph ..."));
    if(mpGraph->empty())
    {
        QMessageBox::information(this, tr("Nothing to clear"), "The graph is empty");
        updateStatus("Done clearing the graph", GraphWidgetManager::TIMEOUT);
    }
    else
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, tr("Confirm clear"), tr("The graph will be erased! Are you sure you want to continue?"), QMessageBox::Yes | QMessageBox::No);
        switch(button)
        {
            case QMessageBox::Yes:
                reset();
                updateStatus("Done clearing the graph", GraphWidgetManager::TIMEOUT);
            break;

            default:
                updateStatus("Clearing the graph aborted by user", GraphWidgetManager::TIMEOUT);
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
                // repositioning node in a scaled fashion
                nodeItem->setPos(mScaleFactor * gvNode.x(), mScaleFactor * gvNode.y());
            }
        }

        // Edge routing not yet available
        // {
        //     using namespace graph_analysis::io;
        //     std::vector<GVEdge> edges = mpGVGraph->edges();
        //     std::vector<GVEdge>::const_iterator cit = edges.begin();
        //     for(; cit != edges.end(); ++cit)
        //     {
        //         GVEdge gvEdge = *cit;
        //         EdgeItem* edgeItem = mEdgeItemMap[ gvEdge.getEdge() ];
        //         edgeItem->setPainterPath( gvEdge.path );
        //     }
        // }
    }
}

void GraphWidget::refresh(bool all)
{
    LOG_DEBUG_S << "Refresh widget: " << getClassName().toStdString();
    reset(true /*keepData*/);

    update();
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
        updateStatus("Dot Graph export failed: " + std::string(e.what()), GraphWidgetManager::TIMEOUT);
    }
}

void GraphWidget::keyPressEvent(QKeyEvent* event)
{
    // check for a keys combination
    Qt::KeyboardModifiers modifiers = event->modifiers();
    if(!mpGraphWidgetManager)
    {
        throw std::runtime_error("graph_analysis::gui::GraphWidget::keyPressEvent: GraphWidgetManager is not set for this widget");
    }

    if(modifiers & Qt::ControlModifier) // key combinations while holding 'CTRL' pressed
    {
        switch(event->key())
        {
            case Qt::Key_Q: // CTRL+Q and CTRL+W terminate the application
            case Qt::Key_W:
                exit(0);
            break;

            case Qt::Key_R: // CTRL+R deletes the graph (it first prompts again the user)
                getGraphWidgetManager()->resetGraph();
            break;

            case Qt::Key_E: // CTRL+S (save) or CTRL+E (export graph) saves the graph to file
            case Qt::Key_S:
                getGraphWidgetManager()->exportGraph();
            break;

            case Qt::Key_A: // CTRL+A prompts the user to add a node
//                if(!mDragDrop)
//                {
//                    addNodeAdhoc();
//                }
            break;

            case Qt::Key_O:
            case Qt::Key_I: // CTRL+O (open) or CTRL+I (input graph)  
                getGraphWidgetManager()->importGraph();
                break;
            case Qt::Key_L:
                // CTRL+L (layout graph) opens a graph from file
                getGraphWidgetManager()->selectLayout();
                break;

            case Qt::Key_P: // CTRL+P reloads the property dialog (if it is currently not running)
                if(!WidgetManager::getInstance()->getPropertyDialog()->isRunning())
                {
                    WidgetManager::getInstance()->getGraphWidgetManager()->reloadPropertyDialog();
                }
            break;
            case Qt::Key_CapsLock: // CTRL+CapsLock or CTRL+D toggles the active mode (drag-n-drop mode v. move-around mode)
            case Qt::Key_D:
                //mDragDrop ? unsetDragDrop() : setDragDrop();
            break;
        }
    }
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

void GraphWidget::selectElement(graph_analysis::GraphElement::Ptr element)
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

void GraphWidget::unselectElement(graph_analysis::GraphElement::Ptr element)
{
    std::vector<GraphElement::Ptr>::iterator it = std::find(mElementSelection.begin(),
            mElementSelection.end(), element);

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

        if(!element )
        { 
            clearElementSelection(); 
            return;
        }

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

        if(getGraphWidgetManager()->getMode() == GraphWidgetManager::CONNECT_MODE)
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


}

void GraphWidget::mouseReleaseEvent(QMouseEvent*)
{
}

void GraphWidget::renameElement(GraphElement::Ptr element, const std::string& label)
{
    element->setLabel(label);

    Vertex::Ptr vertex;
    Edge::Ptr edge;

    if(vertex = boost::dynamic_pointer_cast<Vertex>(element))
    {
        NodeItem* nodeItem = mNodeItemMap[vertex];
        if(!nodeItem)
        {
            std::string error_msg = std::string("graph_analysis::GraphWidget::renameElement: provided vertex '") + vertex->getLabel() + "' is not registered with the GUI";
            LOG_ERROR_S << error_msg;
            throw std::runtime_error(error_msg);
        }
        nodeItem->setLabel(label);
    } else if(edge = boost::dynamic_pointer_cast<Edge>(element))
    {
        EdgeItem* edgeItem = mEdgeItemMap[edge];
        if(!edgeItem)
        {
            std::string error_msg = std::string("graph_analysis::GraphWidget::renameElement: provided edge '") + edge->getLabel() + "' is not registered with the GUI";
            LOG_ERROR_S << error_msg;
            throw std::runtime_error(error_msg);
        }

        edgeItem->setLabel(label);

        //graphitem::edges::EdgeLabel* edgeLabel = (graphitem::edges::EdgeLabel *) edgeItem->getLabel();
        //edgeLabel->setPlainText(QString(label.c_str()));
        //edge->adjustLabel();
    }
}

// EDIT
Vertex::Ptr GraphWidget::addVertex(const std::string& type, const std::string& label, QPoint* position)
{
    // Registering new vertex -- base on the given type
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

    AddGraphElement graphElementDialog(supportedTypes, this);
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
    //Edge::Ptr edge = EdgeTypeManager::getInstance()->createEdge(type, label, sourceVertex, targetVertex);
    Edge::Ptr edge(new Edge(sourceVertex, targetVertex, label));
    mpGraph->addEdge(edge);
    return edge;
}

void GraphWidget::addEdgeDialog(Vertex::Ptr sourceVertex, Vertex::Ptr targetVertex)
{
    updateStatus("Adding edge ...");
    QStringList typesList;
    AddGraphElement graphElementDialog(typesList);// EdgeTypeManager::getInstance()->getSupportedTypes());
    if(graphElementDialog.result() == QDialog::Accepted)
    {
        Edge::Ptr edge = addEdge(graphElementDialog.getType().toStdString(), graphElementDialog.getLabel().toStdString(), sourceVertex, targetVertex);
        updateStatus("Added edge '" + edge->toString() + "' of type '" + edge->getClassName() + "'",
                GraphWidgetManager::TIMEOUT);
        refresh();
    } else 
    {
        updateStatus("Adding edge aborted by user", GraphWidgetManager::TIMEOUT);
    }
}

} // end namespace gui
} // end namespace graph_analysis
