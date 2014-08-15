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
     * \return position of filter
     */
    int add(FilterType::Ptr filter)
    {
        if(filter)
        {
            mFilters.push_back(filter);
            return mFilters.size() - 1;
        } else {
            throw std::runtime_error("Filter: cannot add a null object as filter");
        }
    }

    void removeAt(int position)
    {
        if(position > mFilters.size() -1)
        {
            throw std::runtime_error("Filter: cannot remove filter. Position outside of current list");
        }
        FilterList list;
        for(int i = 0; i < mFilters.size(); ++i)
        {
            if(i != position)
            {
                list.push_back(mFilters[i]);
            }
        }
        mFilters = list;
        //mFilters.erase(mFilters.begin() + position);
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
