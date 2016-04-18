#include "Port.hpp"
#include "HasFeature.hpp"
#include "PortConnection.hpp"
#include "Task.hpp"

#include <base/Logging.hpp>

namespace graph_analysis
{
namespace task_graph
{

Port::Port(const std::string& label)
    : Vertex(label)
{
}

shared_ptr<Task> Port::getTask(const BaseGraph::Ptr& graph) const
{
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr edgeIt = graph->getInEdgeIterator(vertexPtr);
    while(edgeIt->next())
    {
        HasFeature::Ptr feature =
            dynamic_pointer_cast<HasFeature>(edgeIt->current());
        if(!feature)
        {
            continue;
        }
        Task::Ptr comp = dynamic_pointer_cast<Task>(feature->getSourceVertex());
        if(comp)
        {
            return comp;
        }
    }
    LOG_ERROR_S << "Port " << getLabel() << " is not attached to a task?";
    return Task::Ptr();
}

bool Port::isConnected(const BaseGraph::Ptr& graph) const
{
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr edgeIt = graph->getInEdgeIterator(vertexPtr);
    int connections = 0;
    while(edgeIt->next())
    {
        PortConnection::Ptr conn =
            dynamic_pointer_cast<PortConnection>(edgeIt->current());
        if(conn)
        {
            connections++;
        }
    }
    if(connections > 1)
    {
        LOG_ERROR_S << "Port " << getLabel() << " has " << connections
                    << " connections. only one is expected?";
    }
    return connections;
}
}
} // end namespace graph_analysis
