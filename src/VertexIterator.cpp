#include "VertexIterator.hpp"

namespace graph_analysis {

bool VertexIterator::skip(const Vertex::Ptr& vertex) const
{
    if(mSkipFunction)
    {
        return mSkipFunction(vertex);
    }
    return false;
}

} // end namespace graph_analysis
