#ifndef GRAPH_ANALYSIS_PERCOLATION_STRATEGY_HPP
#define GRAPH_ANALYSIS_PERCOLATION_STRATEGY_HPP

#include <graph_analysis/SharedPtr.hpp>
#include <graph_analysis/SubGraph.hpp>
#include <graph_analysis/percolation/Event.hpp>

namespace graph_analysis {
namespace percolation {

class Strategy
{
public:
    typedef shared_ptr<Strategy> Ptr;

    virtual ~Strategy() {}

    /**
     * Apply strategy to given subgraph
     * \return List of change events that have been applied to the graph
     */
    virtual EventList apply(SubGraph::Ptr subgraph) { throw std::runtime_error("graph_analysis::percolation::Strategy::apply has not been implemented"); }
};

} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PERCOLATION_STRATEGY_HPP
