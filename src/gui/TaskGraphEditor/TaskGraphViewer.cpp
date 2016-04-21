#include "TaskGraphViewer.hpp"

#include <exception>
#include <math.h>
#include <set>
#include <sstream>
#include <string>

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QSignalMapper>
#include <QTime>

#include <base/Logging.hpp>
#include <base/Time.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <graph_analysis/io/GVGraph.hpp>

#include <graph_analysis/EdgeTypeManager.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

#include <graph_analysis/task_graph/DataType.hpp>
#include <graph_analysis/task_graph/DataValue.hpp>
#include <graph_analysis/task_graph/HasFeature.hpp>
#include <graph_analysis/task_graph/HasUniqueFeature.hpp>
#include <graph_analysis/task_graph/PortConnection.hpp>
#include <graph_analysis/task_graph/Property.hpp>

#include <graph_analysis/algorithms/BFS.hpp>

#include "PortConnectionItem.hpp"
#include "TaskItem.hpp"

using namespace graph_analysis;

namespace graph_analysis
{
namespace gui
{

TaskGraphViewer::TaskGraphViewer(graph_analysis::BaseGraph::Ptr graph,
                                 QWidget* parent)
    : GraphWidget(graph, parent)
{

    VertexTypeManager* vertexManager = VertexTypeManager::getInstance();
    vertexManager->registerType(task_graph::Task::vertexType(),
                                Vertex::Ptr(new task_graph::Task()));
    vertexManager->registerType(task_graph::InputPort::vertexType(),
                                Vertex::Ptr(new task_graph::InputPort()));
    vertexManager->registerType(task_graph::OutputPort::vertexType(),
                                Vertex::Ptr(new task_graph::OutputPort()));
    vertexManager->registerType(task_graph::Property::vertexType(),
                                Vertex::Ptr(new task_graph::Property()));
    vertexManager->registerType(task_graph::DataType::vertexType(),
                                Vertex::Ptr(new task_graph::DataType()));
    vertexManager->registerType(task_graph::DataValue::vertexType(),
                                Vertex::Ptr(new task_graph::DataValue()));

    EdgeTypeManager* edgeManager = EdgeTypeManager::getInstance();
    edgeManager->registerType(task_graph::HasFeature::edgeType(),
                              Edge::Ptr(new task_graph::HasFeature()));
    edgeManager->registerType(task_graph::HasUniqueFeature::edgeType(),
                              Edge::Ptr(new task_graph::HasUniqueFeature()));
    edgeManager->registerType(task_graph::PortConnection::edgeType(),
                              Edge::Ptr(new task_graph::PortConnection()));
}

TaskGraphViewer::~TaskGraphViewer()
{
}

QString TaskGraphViewer::getClassName() const
{
    return "graph_analysis::gui::TaskGraphViewer";
}

// differs from the base-implementation in that here, only things of type
// "TaskItem" are shuffled.
// Uses an archimedal spiral to layout the things :)
// TODO:
// * Should incorporate the size of nodes
// * Parameters should be formalized (especially alpha update)
// * alpha has to increase with a delta decreasing faster than 1/x
void TaskGraphViewer::shuffle()
{
    double alpha = M_PI / 2;
    double scale = 400 / (M_PI * 2);

    foreach(QGraphicsItem* item, scene()->items())
    {
        if(dynamic_cast<TaskItem*>(item))
        {
            // Set item pos in a spiral manner
            double x = scale * alpha * cos(alpha);
            double y = scale * alpha * sin(alpha);
            std::cout << x << "," << y << std::endl;
            item->setPos(x, y);
            // Update alpha
            alpha += ((M_PI / 2.) / (alpha/10. + 1.));
            //alpha += (M_PI / 2.);
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

        TaskItem* v = new TaskItem(this, comp, NULL);
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
        PortConnectionItem* e = new PortConnectionItem(this, conn, NULL);
        scene()->addItem(e);
    }
}

} // end namespace gui
} // end namespace graph_analysis
