#include "PortConnection.hpp"

#include <base/Logging.hpp>

namespace graph_analysis
{

PortConnection::PortConnection(const OutputPort::Ptr &output,
                               const InputPort::Ptr &input,
                               const std::string &label)
    : Edge(output, input, label)
{
}

OutputPort::Ptr PortConnection::getSourcePort(const BaseGraph::Ptr &graph) const
{
    Edge::Ptr edgePtr = getSharedPointerFromGraph(graph);
    return dynamic_pointer_cast<OutputPort>(edgePtr->getSourceVertex());
}

InputPort::Ptr PortConnection::getTargetPort(const BaseGraph::Ptr &graph) const
{
    Edge::Ptr edgePtr = getSharedPointerFromGraph(graph);
    return dynamic_pointer_cast<InputPort>(edgePtr->getTargetVertex());
}

} // end namespace graph_analysis
