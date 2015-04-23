#include "EdgeIterable.hpp"

namespace graph_analysis {

uint64_t EdgeIterable::getEdgeCount()
{
    uint64_t count = 0;
    EdgeIterator::Ptr edgeIt = getEdgeIterator();
    while(edgeIt->next())
    {
        ++count;
    }
    return count;
}

} // end namespace graph_analysis
