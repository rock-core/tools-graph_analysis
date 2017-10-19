#include "UniformRandomEdgeRemoval.hpp"
#include "../RandomNumberGenerator.hpp"

namespace graph_analysis {
namespace percolation {
namespace strategies {

UniformRandomEdgeRemoval::UniformRandomEdgeRemoval(double occupationProbability, uint32_t seed)
    : mOccupationProbability(occupationProbability)
    , mpRandomNumberGenerator(new RandomNumberGenerator(seed))
{
}

EventList UniformRandomEdgeRemoval::apply(SubGraph::Ptr subgraph)
{
    EventList eventList;

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

            Event event(Event::REMOVE_EDGE, edge);
            eventList.push_back(event);
        }
    }

    return eventList;
}

} // end namespace strategies
} // end namespace percolation
} // end namespace graph_analysis
