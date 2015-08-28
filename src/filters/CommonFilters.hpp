#ifndef GRAPH_ANALYSIS_COMMON_FILTERS_HPP
#define GRAPH_ANALYSIS_COMMON_FILTERS_HPP

#include <graph_analysis/Filter.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>

namespace graph_analysis {
namespace filters {

/**
 * Filter out all items
 */
template<typename FilterObject>
class DenyAll : public Filter<FilterObject>
{
    virtual std::string getName() const { return "graph_analysis::filter::DenyAll"; }
    virtual bool apply(FilterObject o ) const { return true; }
};

/**
 * Filter out all edges
 * \brief Inherits from edge context filter to make sure any subfilters of type EdgeContextFilter are properly applied
 */
template<>
class DenyAll<graph_analysis::Edge::Ptr> : public EdgeContextFilter
{
    virtual std::string getName() const { return "graph_analysis::filter::DenyAll (Edges)"; }
    virtual bool apply(graph_analysis::Edge::Ptr e) const { return true; }
    virtual bool evaluateTarget(graph_analysis::Edge::Ptr e) const { return true; }
    virtual bool evaluateSource(graph_analysis::Edge::Ptr e) const { return true; }
};

/**
 * Filter no items -- seems to be a pretty useless filter ;) i.e. for testing purposes only
 */
template<typename FilterObject>
class PermitAll : public Filter<FilterObject>
{
    virtual std::string getName() const { return "graph_analysis::filter::PermitAll"; }
    virtual bool apply(FilterObject o ) const { return false; }
};

/**
 * Filter that permits all edges
 * \brief Inherits from edge context filter to make sure any subfilters of type EdgeContextFilter are properly applied
 */
template<>
class PermitAll<graph_analysis::Edge::Ptr> : public EdgeContextFilter
{
    virtual std::string getName() const { return "graph_analysis::filter::PermitAll (Edges)"; }
    virtual bool apply(graph_analysis::Edge::Ptr e) const { return false; }
    virtual bool evaluateTarget(graph_analysis::Edge::Ptr e) const { return false; }
    virtual bool evaluateSource(graph_analysis::Edge::Ptr e) const { return false; }
};

} // end namespace filter
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_COMMON_FILTERS_HPP
