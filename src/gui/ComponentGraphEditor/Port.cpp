#include "Port.hpp"

#include "HasFeature.hpp"
#include <graph_analysis/gui/ComponentGraphEditor/Component.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/PortConnection.hpp>

#include <base/Logging.hpp>

namespace graph_analysis {

Port::Port(const std::string& label)
    : Vertex(label)
{}

shared_ptr<Component> Port::getComponent(const BaseGraph::Ptr &graph) const
{
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr edgeIt = graph->getInEdgeIterator(vertexPtr);
    while(edgeIt->next())
    {
        graph_analysis::HasFeature::Ptr feature =
            dynamic_pointer_cast<HasFeature>(edgeIt->current());
        if(!feature)
        {
            continue;
        }
        graph_analysis::Component::Ptr comp =
            dynamic_pointer_cast<Component>(feature->getSourceVertex());
        if(comp)
        {
            return comp;
        }
    }
    LOG_ERROR_S << "Port " << getLabel() << " is not attached to a component?";
    return Component::Ptr();
}

bool Port::isConnected(const BaseGraph::Ptr& graph) const
{
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr edgeIt = graph->getInEdgeIterator(vertexPtr);
    int connections = 0;
    while(edgeIt->next())
    {
        graph_analysis::PortConnection::Ptr conn =
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

} // end namespace graph_analysis
