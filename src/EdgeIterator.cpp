#include "EdgeIterator.hpp"

namespace graph_analysis {

bool EdgeIterator::skip(Edge::Ptr edge) const
{
    if(mSkipFunction)
    {
        return mSkipFunction(edge);
    }
    return false;
}

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
