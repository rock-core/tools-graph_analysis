#include "UniformRandomVertexRemoval.hpp"
#include <graph_analysis/percolation/RandomNumberGenerator.hpp>

namespace graph_analysis {
namespace percolation {
namespace strategies {

UniformRandomVertexRemoval::UniformRandomVertexRemoval(double occupationProbability, uint32_t seed)
    : mOccupationProbability(occupationProbability)
    , mpRandomNumberGenerator(new RandomNumberGenerator(seed))
{
}

void UniformRandomVertexRemoval::apply(SubGraph::Ptr subgraph)
{
    mDisabled.clear();

    VertexIterator::Ptr vertexIterator = subgraph->getVertexIterator();
    while(vertexIterator->next())
    {
        Vertex::Ptr vertex = vertexIterator->current();

        if(mOccupationProbability > mpRandomNumberGenerator->getUniformPositiveNumber())
        {
            // the are occupied
        } else {
            subgraph->disable(vertex);
            mDisabled.push_back(vertex);
        }
    }
}

} // end namespace strategies
} // end namespace percolation
} // end namespace graph_analysis
