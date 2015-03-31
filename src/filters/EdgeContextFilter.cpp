#include "EdgeContextFilter.hpp"

namespace graph_analysis {
namespace filters {

bool EdgeContextFilter::permitsTarget(graph_analysis::Edge::Ptr e) const
{
    if(evaluateTarget(e))
    {
        return true;
    }

    typename FilterList::const_iterator cit = mFilters.begin();
    for(; cit != mFilters.end(); ++cit)
    {
        Ptr edgeFilter = boost::dynamic_pointer_cast<EdgeContextFilter>(*cit);
        if(edgeFilter && edgeFilter->permitsTarget(e) )
        {
            return true;
        }
    }
    return false;
}

bool EdgeContextFilter::permitsSource(graph_analysis::Edge::Ptr e) const
{
    if(evaluateSource(e))
    {
        return true;
    }

    typename FilterList::const_iterator cit = mFilters.begin();
    for(; cit != mFilters.end(); ++cit)
    {
        Ptr edgeFilter = boost::dynamic_pointer_cast<EdgeContextFilter>(*cit);
        if(edgeFilter && edgeFilter->permitsSource(e) )
        {
            return true;
        }
    }
    return false;
}

} // end namespace filters
} // end namespace graph_analysis
