#include "Percolation.hpp"

namespace graph_analysis {
namespace percolation {

Percolation::Percolation(BaseGraph::Ptr graph)
    : mpGraph(graph)
    , mpSubGraph(BaseGraph::getSubGraph(graph))
{}

void Percolation::reset()
{
    mpSubGraph->enableAllVertices();
    mpSubGraph->enableAllEdges();
}

EventList Percolation::apply(Strategy::Ptr strategy)
{
    return strategy->apply(mpSubGraph);
}

} // end percolation
} // end namespace 
