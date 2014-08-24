#ifndef GRAPH_ANALYSIS_FILTER_HPP
#define GRAPH_ANALYSIS_FILTER_HPP

#include <map>
#include <vector>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <graph_analysis/Edge.hpp>

namespace graph_analysis {

namespace filters {

enum Type { CONTENT = 0, CLASS };
extern std::map<Type, std::string> TypeTxt;
extern std::map<std::string, Type> TxtType;
} // end namespace filters

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

    virtual ~Filter() {}

    /**
     * \brief Get Name of the filter
     * \return name of the filter
     */
    virtual std::string getName() const { return "graph_analysis::Filter"; }

    /**
     * \brief Get string representation of filter
     * \return name by default
     */
    virtual std::string toString() const { return getName(); }

    /**
     * Replace a filter at a certain position
     * \param filter Filter to replace the existing one with
     * \param position Position of the filter that should be replaced
     * \throws std::out_of_range if the position does not exist
     */
    int replace(FilterType::Ptr filter, int position = -1)
    {
        if(position > -1)
        {
            // trigger out of range exception if position does not exist
            FilterType::Ptr filterAtPosition = mFilters.at(position);
            removeAt(position);
        }
        return add(filter, position);
    }

    /**
     * Clears the list of additional filters that have been added to this filter
     */
    void clear()
    {
        mFilters.clear();
    }

    /**
     * Add a filter
     * \param filter Filter to  add
     * \param position Position of the filter
     * \return position of filter
     */
    int add(FilterType::Ptr filter, int position = -1)
    {
        if(filter)
        {
            if(position > -1)
            {
                mFilters.insert(mFilters.begin() + position, filter);
                return position;
            } else {
                mFilters.push_back(filter);
                return mFilters.size() - 1;
            }
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
     * \brief Apply all filters, i.e. root filter and children
     * to the filter object
     * \param o FilterObject, i.e. target that requires evaluation
     * \return True if this item is matched by any of the filters, false otherwise
     */
    bool evaluate(FilterObject o) const
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
            if( filter->evaluate(o) )
            {
                return true;
            }
        }
        return false;
    }

    virtual bool filterTarget(graph_analysis::Edge::Ptr e) const { throw std::runtime_error("graph_analysis::Filter::filterTarget is not implemented for: " + toString()); }
    virtual bool filterSource(graph_analysis::Edge::Ptr e) const { throw std::runtime_error("graph_analysis::Filter::filterSource is not implemented for: " + toString()); }

    /**
     * \brief Apply only the main filter to the target object
     * \param o Filter object, i.e. target object to apply the filter on
     * \return True, if the main filter matches the target/filter object, false otherwise
     */
    virtual bool apply(FilterObject o) const { return false; }

private:
    FilterList mFilters;
};

class EdgeFilter : public Filter< graph_analysis::Edge::Ptr >
{
public:
    virtual bool filterTarget(graph_analysis::Edge::Ptr e) const { return false; }
    virtual bool filterSource(graph_analysis::Edge::Ptr e) const { return true; }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_FILTER_HPP
