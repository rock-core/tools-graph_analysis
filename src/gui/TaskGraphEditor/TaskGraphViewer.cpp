#include "TaskGraphViewer.hpp"

#include <exception>
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

#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <base/Time.hpp>
#include <base/Logging.hpp>

#include <graph_analysis/io/GVGraph.hpp>

#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <graph_analysis/task_graph/PortConnection.hpp>
#include <graph_analysis/task_graph/HasFeature.hpp>

#include "TaskItem.hpp"
#include "PortConnectionItem.hpp"

using namespace graph_analysis;

namespace graph_analysis
{
namespace gui
{

TaskGraphViewer::TaskGraphViewer(graph_analysis::BaseGraph::Ptr graph, QWidget *parent) : GraphWidget(graph, parent)
{

    VertexTypeManager* vertexManager = VertexTypeManager::getInstance();
    vertexManager->registerType(task_graph::Task::vertexType(), Vertex::Ptr(new task_graph::Task()));
    vertexManager->registerType(task_graph::InputPort::vertexType(), Vertex::Ptr(new task_graph::InputPort()));
    vertexManager->registerType(task_graph::OutputPort::vertexType(), Vertex::Ptr(new task_graph::OutputPort()));

    EdgeTypeManager* edgeManager = EdgeTypeManager::getInstance();
    edgeManager->registerType(task_graph::HasFeature::edgeType(), Edge::Ptr(new task_graph::HasFeature()));
    edgeManager->registerType(task_graph::PortConnection::edgeType(), Edge::Ptr(new task_graph::PortConnection()));
}

TaskGraphViewer::~TaskGraphViewer() {}

QString TaskGraphViewer::getClassName() const
{
    return "graph_analysis::gui::TaskGraphViewer";
}

// differs from the base-implementation in that here, only things of type
// "TaskItem" are shuffled.
void TaskGraphViewer::shuffle()
{
    int diff = 600 * mpGraph->order();
    foreach(QGraphicsItem *item, scene()->items())
    {
        if(dynamic_cast<TaskItem *>(item)) {
            item->setPos(-diff/2 + qrand() % diff, -diff/2 + qrand() % diff);
        }
    }
    updateStatus(
        "Shuffelled all nodes representing a 'Vertex' of the TaskGraphViewer");
}

void TaskGraphViewer::populateCanvas()
{
    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // this widget only knows one type of "top level item", and this is the
        // Task. so creating then:
        graph_analysis::task_graph::Task::Ptr comp =
            dynamic_pointer_cast<task_graph::Task>(vertex);
        if(!comp)
        {
            continue;
        }

        TaskItem *v = new TaskItem(this, comp, NULL);
        scene()->addItem(v);
    }

    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        graph_analysis::task_graph::PortConnection::Ptr conn =
            dynamic_pointer_cast<task_graph::PortConnection>(edge);
        if(!conn)
        {
            continue;
        }

        // creating new edge items
        PortConnectionItem *e = new PortConnectionItem(
            this, conn, NULL);
        scene()->addItem(e);
    }
}

} // end namespace gui
} // end namespace graph_analysis
