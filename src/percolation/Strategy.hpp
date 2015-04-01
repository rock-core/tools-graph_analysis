#ifndef GRAPH_ANALYSIS_PERCOLATION_STRATEGY_HPP
#define GRAPH_ANALYSIS_PERCOLATION_STRATEGY_HPP

#include <boost/shared_ptr>
#include <graph_analysis/SubGraph.hpp>

namespace graph_analysis {
namespace percolation {

class Strategy
{
public:
    typedef boost::shared_ptr<Strategy> Ptr;

    /**
     * Apply strategy to given subgraph
     */
    virtual void apply(SubGraph::Ptr subgraph) { throw std::runtime_error("graph_analysis::percolation::Strategy::apply has not been implemented"); }
};

} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PERCOLATION_STRATEGY_HPP
