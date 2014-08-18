#ifndef GRAPH_ANALYSIS_COMMON_FILTERS_HPP
#define GRAPH_ANALYSIS_COMMON_FILTERS_HPP

#include <graph_analysis/Filter.hpp>

namespace graph_analysis {
namespace filters {

/**
 * Filter all items
 */
template<typename FilterObject>
class DenyAll : public Filter<FilterObject>
{
    virtual std::string getName() const { return "graph_analysis::filter::DenyAll"; }
    virtual bool apply(FilterObject o ) { return true; }
};

/**
 * Filter no items -- seems to be a pretty useless filter ;) i.e. for testing purposes only
 */
template<typename FilterObject>
class PermitAll : public Filter<FilterObject>
{
    virtual std::string getName() const { return "graph_analysis::filter::PermitAll"; }
    virtual bool apply(FilterObject o ) { return false; }
};

} // end namespace filter
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_COMMON_FILTERS_HPP
