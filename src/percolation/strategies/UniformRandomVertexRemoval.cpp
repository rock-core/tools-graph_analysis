#include "UniformRandomVertexRemoval.hpp"
#include "../RandomNumberGenerator.hpp"

namespace graph_analysis {
namespace percolation {
namespace strategies {

UniformRandomVertexRemoval::UniformRandomVertexRemoval(double occupationProbability, uint32_t seed)
    : mOccupationProbability(occupationProbability)
    , mpRandomNumberGenerator(new RandomNumberGenerator(seed))
{
}

EventList UniformRandomVertexRemoval::apply(SubGraph::Ptr subgraph)
{
    EventList eventList;
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

            Event event(Event::REMOVE_VERTEX, vertex);
            eventList.push_back(event);
        }
    }

    return eventList;
}

} // end namespace strategies
} // end namespace percolation
} // end namespace graph_analysis
