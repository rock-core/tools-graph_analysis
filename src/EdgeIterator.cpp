#include "EdgeIterator.hpp"

namespace graph_analysis {

bool EdgeIterator::skip(const Edge::Ptr& edge) const
{
    if(mSkipFunction)
    {
        return mSkipFunction(edge);
    }
    return false;
}

} // end namespace graph_analysis
