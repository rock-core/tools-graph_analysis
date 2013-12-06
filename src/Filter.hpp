#ifndef GRAPH_ANALYSIS_FILTER_HPP
#define GRAPH_ANALYSIS_FILTER_HPP

#include <vector>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

namespace graph_analysis {

/**
 * Allow definition of a filter per object type
 *
 * This filter class allows bagging of multiple individual filters into a single
 * filter object
 */
template<typename FilterObject>
class Filter
{
public:
    typedef Filter<FilterObject> FilterType;
    typedef boost::shared_ptr<FilterType> Ptr;

    typedef std::vector< FilterType::Ptr > FilterList;

    /**
     * \brief Get Name of the filter
     */
    std::string getName() const { return "graph_analysis::Filter"; }

    /**
     * Add a filter
     */
    void add(FilterType::Ptr filter)
    {
        if(filter)
        {
            mFilters.push_back(filter);
        } else {
            throw std::runtime_error("Filter: cannot add a null object as filter");
        }
    }

    /**
     * \brief Apply all filters
     */
    bool evaluate(FilterObject o)
    {
        // Check for main filter
        if( apply(o) )
        {
            return true;
        }

        typename FilterList::const_iterator cit = mFilters.begin();
        for(; cit != mFilters.end(); ++cit)
        {
            FilterType::Ptr filter = *cit;
            assert(filter);
            if( filter->apply(o) )
            {
                return true;
            }
        }
        return false;
    }

    /**
     * \brief Apply the filter to the target object
     */
    virtual bool apply(FilterObject o) { return false; }

private:
    FilterList mFilters;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_FILTER_HPP
