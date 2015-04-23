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

} // end namespace graph_analysis
