#include "Port.hpp"
#include "HasFeature.hpp"
#include "PortConnection.hpp"
#include "Task.hpp"
#include "DataType.hpp"
#include "HasUniqueFeature.hpp"

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

shared_ptr<DataType> Port::getDataType(const BaseGraph::Ptr& graph) const
{
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr edgeIt = graph->getOutEdgeIterator(vertexPtr);
    while(edgeIt->next())
    {

        HasUniqueFeature::Ptr feature =
            dynamic_pointer_cast<HasUniqueFeature>(edgeIt->current());
        if(!feature)
        {
            continue;
        }
        DataType::Ptr data = dynamic_pointer_cast<DataType>(
            edgeIt->current()->getTargetVertex());
        if(data)
        {
            return data;
        }
    }
    return shared_ptr<DataType>();
}

bool Port::isOwnDataTypeSameAs(const BaseGraph::Ptr& graph,
                                     const shared_ptr<Port> port) const
{
    Port::Ptr ownPtr =
        dynamic_pointer_cast<Port>(getSharedPointerFromGraph(graph));
    if(!ownPtr)
    {
        LOG_ERROR_S << "unexpected self, am not a 'Port'. fix this!";
        return false;
    }

    return ownPtr->getDataType(graph)->toString() ==
           port->getDataType(graph)->toString();
}

bool Port::checkIfPortConnectionWouldBeLegal(
    const BaseGraph::Ptr& graph,
    const graph_analysis::task_graph::Port::Ptr otherPort) const
{
    graph_analysis::task_graph::Port::Ptr thisPort =
        dynamic_pointer_cast<graph_analysis::task_graph::Port>(
            getSharedPointerFromGraph(graph));

    if(otherPort->getTask(graph) == thisPort->getTask(graph))
    {
        LOG_INFO_S << "proposed port connection illegal, Ports are part of the "
                      "same Task";
        return false;
    }
    else if(!isOwnDataTypeSameAs(graph, otherPort))
    {
        LOG_INFO_S
        << "proposed port connection illegal, DataType does not match";
        return false;
    }
    else if(thisPort->isConnected(graph))
    {
        LOG_INFO_S
        << "proposed port connection illegal, this Port is already connected";
        return false;
    }
    else if(otherPort->isConnected(graph))
    {
        LOG_INFO_S
        << "proposed port connection illegal, other Port is already connected";
        return false;
    }
    else if(dynamic_pointer_cast<graph_analysis::task_graph::OutputPort>(
                thisPort) &&
            dynamic_pointer_cast<graph_analysis::task_graph::OutputPort>(
                otherPort))
    {
        LOG_INFO_S
        << "proposed port connection illegal, both ports are OutputPort";
        return false;
    }
    else if(dynamic_pointer_cast<graph_analysis::task_graph::InputPort>(
                thisPort) &&
            dynamic_pointer_cast<graph_analysis::task_graph::InputPort>(
                otherPort))
    {
        LOG_INFO_S
        << "proposed port connection illegal, both ports are InputPort";
        return false;
    }
    LOG_INFO_S << "proposed port connection LEGAL, evetything alright";
    return true;
}
}
} // end namespace graph_analysis
