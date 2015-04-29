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

} // end namespace graph_analysis
