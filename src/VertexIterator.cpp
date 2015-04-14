#include "VertexIterator.hpp"

namespace graph_analysis {

bool VertexIterator::skip(Vertex::Ptr vertex) const
{
    if(mSkipFunction)
    {
        return mSkipFunction(vertex);
    }
    return false;
}

uint64_t VertexIterable::getVertexCount()
{
    uint64_t count = 0;
    VertexIterator::Ptr vertexIt = getVertexIterator();
    while(vertexIt->next())
    {
        ++count;
    }
    return count;
}

} // end namespace graph_analysis
