#ifndef GRAPH_ANALYSIS_FILTER_HPP
#define GRAPH_ANALYSIS_FILTER_HPP

#include <vector>
#include <boost/shared_ptr.hpp>

namespace graph_analysis {

/**
 * Allow definition of a filter per object type
 */
template<typename FilterObject>
class Filter
{
public:
    typedef Filter<FilterObject> FilterType;
    typedef boost::shared_ptr<FilterType> Ptr;

    typedef std::vector< FilterType::Ptr > FilterList;

    /**
     * Add a filter
     */
    void add(FilterType::Ptr filter) { mFilters.push_back(filter); }

    /**
     * Apply all filters
     */
    bool evaluate(FilterObject o)
    {
        typename FilterList::const_iterator cit = mFilters.begin();
        for(; cit != mFilters.end(); ++cit)
        {
            if((*cit)->apply(o))
            {
                return true;
            }
        }
        return false;
    }

    virtual bool apply(FilterObject o) { return false; }

private:
    FilterList mFilters;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_FILTER_HPP
