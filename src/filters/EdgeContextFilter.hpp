#ifndef GRAPH_ANALYSIS_FILTERS_EDGE_CONTEXT_FILTER_HPP
#define GRAPH_ANALYSIS_FILTERS_EDGE_CONTEXT_FILTER_HPP

#include <graph_analysis/Filter.hpp>

namespace graph_analysis {
namespace filters {
/**
 * Interface definition for an edge context filter
 * which allows to define whether the target / source node of a specific edge should be filtered
 */
class EdgeContextFilter : public Filter<graph_analysis::Edge::Ptr>
{
public:
    typedef shared_ptr<EdgeContextFilter> Ptr;

    /**
     * \brief Get Name of the filter
     * \return name of the filter
     */
    virtual std::string getName() const { return "graph_analysis::EdgeContextFilter"; }

    /**
     * \brief Get string representation of filter
     * \return name by default
     */
    virtual std::string toString() const { return getName(); }

    /**
     * \brief Evaluated the target of the edge
     * \return True if it should be permitted, false otherwise
     */
    virtual bool evaluateTarget(graph_analysis::Edge::Ptr e) const { return false; }

    /**
     * \brief Use this and associated subfilters, to check if this target vertex
     * is matched by the filter
     * \return True, if the target vertex of given edge is matched by the filter, false otherwise
     */
    bool matchesTarget(graph_analysis::Edge::Ptr e) const;

    /**
     * \brief Evaluated the source vertex of the edge
     * \return True if it should be filtered, false otherwise
     */
    virtual bool evaluateSource(graph_analysis::Edge::Ptr e) const { return false; }

    /**
     * \brief Use this and associated subfilters, to check if this edge's source vertex is matched by the filter
     * \return True, if the source vertex of given edge is matched by the filter, false otherwise
     */
    bool matchesSource(graph_analysis::Edge::Ptr e) const;
};

} // end namespace filters
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_FILTERS_EDGE_CONTEXT_FILTER_HPP
