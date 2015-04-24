#ifndef GRAPH_ANALYSIS_FILTER_HPP
#define GRAPH_ANALYSIS_FILTER_HPP

#include <map>
#include <vector>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <graph_analysis/Edge.hpp>

namespace graph_analysis {
namespace filters {

/// Operator 
enum Operator { AND = 0, OR };
/// Filters can operator on content or class information
enum Type { CONTENT = 0, CLASS };
extern std::map<Type, std::string> TypeTxt;
extern std::map<std::string, Type> TxtType;

} // end namespace filters

/**
 * Allow definition of a filter per object type.
 * Object type should inherit from Vertex or Edge
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
     * Deconstructor
     */
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
     * Replace a filter at a certain position in the internally kept filter list
     * This might be needed to update a filter
     * \param filter Filter to replace the existing one with
     * \param position Position of the filter that should be replaced, if
     * position -1 is given the filter is added without replacing another
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
     * \param filter Filter to add
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
    }

    /**
     * \brief Apply all filters, i.e. root filter and children
     * to the filter object, i.e., test whether filter permits filter object
     * \param o FilterObject, i.e. target that requires evaluation
     * \param op Chain the subfilter dis/con-junctively using available
     * operators OR and AND
     * \return True if this item is matched and permitted by any of the filters, false otherwise
     */
    bool permits(FilterObject o, filters::Operator op = filters::OR) const
    {

        bool result = apply(o);

        typename FilterList::const_iterator cit = mFilters.begin();
        if(op == filters::OR)
        {
            if(result)
            {
                return true;
            }

            for(; cit != mFilters.end(); ++cit)
            {

                FilterType::Ptr filter = *cit;
                assert(filter);
                if(filter->permits(o))
                {
                    return true;
                }
            }
            result = false;

        } else if(op == filters::AND)
        {
            if(!result)
            {
                return false;
            }

            for(; cit != mFilters.end(); ++cit)
            {
                FilterType::Ptr filter = *cit;
                assert(filter);
                // filter should be set to true here, otherwise false
                // will be returned
                if(!filter->permits(o))
                {
                    return false;
                }
            }
        }
        return result;
    }

    /**
     * \brief Apply only the main filter to the target object
     * \param o Filter object, i.e. target object to apply the filter on
     * \return True, if the main filter matches the target/filter object, false otherwise
     */
    virtual bool apply(FilterObject o) const { return false; }

    /**
     * Return Null Filter
     */
    static Ptr Null() { return Ptr(); }

protected:
    FilterList mFilters;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_FILTER_HPP
