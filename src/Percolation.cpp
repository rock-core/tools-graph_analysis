#include "Percolation.hpp"

namespace graph_analysis {
namespace percolation {

Percolation::Percolation(BaseGraph::Ptr graph)
    : mpGraph(graph)
    , mpSubGraph(graph.getSubGraph())
{}

SubGraph::Ptr Percolation::triggerRemoval(const Strategy& strategy)
{
    strategy.apply(mpSubGraph);
    return mpSubGraph;
}

} // end percolation
} // end namespace 
