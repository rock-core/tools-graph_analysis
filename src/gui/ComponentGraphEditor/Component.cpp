#include "Component.hpp"
#include "HasFeature.hpp"

#include <base/Logging.hpp>

namespace graph_analysis
{

Component::Component(const std::string &label) : Vertex(label) {}

std::vector<OutputPort::Ptr>
Component::getOutputPorts(const BaseGraph::Ptr &graph) const
{
    std::vector<OutputPort::Ptr> retval;
    // obtain the shared pointer of "this"...
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr edgeIt = graph->getOutEdgeIterator(vertexPtr);
    while(edgeIt->next())
    {
        graph_analysis::HasFeature::Ptr feature =
            dynamic_pointer_cast<HasFeature>(edgeIt->current());
        if(!feature)
        {
            continue;
        }

        // creating new node items
        graph_analysis::OutputPort::Ptr port =
            dynamic_pointer_cast<OutputPort>(feature->getTargetVertex());
        if(!port)
        {
            continue;
        }

        retval.push_back(port);
    }
    return retval;
}

std::vector<InputPort::Ptr>
Component::getInputPorts(const BaseGraph::Ptr &graph) const
{
    std::vector<InputPort::Ptr> retval;
    // obtain the shared pointer of "this"...
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr edgeIt = graph->getOutEdgeIterator(vertexPtr);
    while(edgeIt->next())
    {
        graph_analysis::HasFeature::Ptr feature =
            dynamic_pointer_cast<HasFeature>(edgeIt->current());
        if(!feature)
        {
            continue;
        }

        // creating new node items
        graph_analysis::InputPort::Ptr port =
            dynamic_pointer_cast<InputPort>(feature->getTargetVertex());
        if(!port)
        {
            continue;
        }

        retval.push_back(port);
    }
    return retval;
}

} // end namespace graph_analysis
