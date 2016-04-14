#include "Property.hpp"
#include "HasUniqueFeature.hpp"

namespace graph_analysis
{
namespace task_graph {

Property::Property(const std::string& label)
    : Vertex(label)
{
}

// TODO: Might be good in the future for automatic graph checking
// For a property this has to be exactly 1. But it might be nice to check in HasUniqueFeature constructor?
//int Property::getRelations(const BaseGraph::Ptr& graph) const
//{
//}

Vertex::Ptr Property::getOwner(const BaseGraph::Ptr& graph) const
{
    Vertex::Ptr owner;
    Vertex::Ptr me = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr eit = graph->getOutEdgeIterator(me);
    while (eit->next())
    {
        if (eit->current()->getClassName() != HasUniqueFeature::edgeType())
            continue;

        if (owner)
            throw std::runtime_error("Property::getOwner: Multiple owners detected");

        owner = eit->current()->getTargetVertex();
    }

    return owner;
}

}
}
