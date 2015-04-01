#include "UniformRandomEdgeRemoval.hpp"
#include <graph_analysis/percolation/RandomNumberGenerator.hpp>

namespace graph_analysis {
namespace percolation {
namespace strategies {

UniformRandomEdgeRemoval::UniformRandomEdgeRemoval(double occupationProbability, uint32_t seed)
    : mOccupationProbability(occupationProbability)
    , mpRandomNumberGenerator(new RandomNumberGenerator(seed))
{
}

void UniformRandomEdgeRemoval::apply(SubGraph::Ptr subgraph)
{
    mDisabled.clear();

    EdgeIterator::Ptr edgeIterator = subgraph->getEdgeIterator();
    while(edgeIterator->next())
    {
        Edge::Ptr edge = edgeIterator->current();

        if(mOccupationProbability > mpRandomNumberGenerator->getUniformPositiveNumber())
        {
            // the are occupied
        } else {
            subgraph->disable(edge);
            mDisabled.push_back(edge);
        }
    }
}

} // end namespace strategies
} // end namespace percolation
} // end namespace graph_analysis
