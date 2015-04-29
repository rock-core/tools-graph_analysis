#include "VertexIterable.hpp"

namespace graph_analysis {

VertexIterable::~VertexIterable()
{}

uint64_t VertexIterable::getVertexCount() const
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

