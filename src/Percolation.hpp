#ifndef GRAPH_ANALYSIS_PERCOLATION_HPP
#define GRAPH_ANALYSIS_PERCOLATION_HPP

/**
 * \namespace percolation
 * \brief Percolation deals with a changing graph, i.e., nodes and edges that
 *        disappear from the graph
 */

#include "BaseGraph.hpp"
#include "SubGraph.hpp"
#include "percolation/Strategy.hpp"

namespace graph_analysis {
namespace percolation {

class Percolation
{
    BaseGraph::Ptr mpGraph;
    SubGraph::Ptr mpSubGraph;

public:
    Percolation(BaseGraph::Ptr graph);

    virtual ~Percolation() {}

    /**
     * Reset the percolation state of the graph
     */
    void reset();

    /**
     * Apply a strategy for percolation, i.e., simulating the removal of nodes
     * \return the current subgraph
     */
    EventList apply(Strategy::Ptr strategy);

    /**
     * Get the current subgraph
     * \return subgraph
     */
    SubGraph::Ptr getSubGraph() { return mpSubGraph; }
};

} // end namespace percolation
} // end namespace graph_analysis
#endif  // GRAPH_ANALYSIS_PERCOLATION_HPP
