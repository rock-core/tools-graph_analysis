#include "Percolation.hpp"

namespace graph_analysis {
namespace percolation {

Percolation::Percolation(BaseGraph::Ptr graph)
    : mpGraph(graph)
    , mpSubGraph(graph->getSubGraph())
{}

void Percolation::reset()
{
    mpSubGraph->enableAllVertices();
    mpSubGraph->enableAllEdges();
}

SubGraph::Ptr Percolation::apply(Strategy::Ptr strategy)
{
    strategy->apply(mpSubGraph);
    return mpSubGraph;
}

} // end percolation
} // end namespace 
