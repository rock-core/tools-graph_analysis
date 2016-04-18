#include "DataType.hpp"
#include "HasUniqueFeature.hpp"
#include "Task.hpp"

namespace graph_analysis
{
namespace task_graph {

DataType::DataType(const std::string& label)
    : Vertex(label)
{
}

Vertex::Ptr DataType::getOwner(const BaseGraph::Ptr& graph) const
{
    Vertex::Ptr owner;
    Vertex::Ptr me = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr eit = graph->getInEdgeIterator(me);
    while (eit->next())
    {
        if (eit->current()->getClassName() != HasUniqueFeature::edgeType())
            continue;

        if (owner)
            throw std::runtime_error("DataType::getOwner: Multiple owners detected");

        owner = eit->current()->getSourceVertex();

        if (owner->getClassName() == Task::vertexType())
            return Vertex::Ptr();
    }

    return owner;
}

}
}
