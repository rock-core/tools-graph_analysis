#ifndef GRAPH_ANALYSIS_ALGORITHMS_MIN_COST_FLOW_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_MIN_COST_FLOW_HPP

#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/NWeightedEdge.hpp>
#include <graph_analysis/WeightedVertex.hpp>

namespace graph_analysis {
namespace algorithms {

/**
 * MinCostFlow allows to use existing algorithms for min-cost flow optimization
 * to be performed on an existing graph
 *
 * The constructed graph should consist of edges of type: NWeightedEdge<uint32_t,3>
 * and supply/demand nodes: WeightedVertex (will use 'int' value though)
 *
 */
class MinCostFlow
{
public:
    typedef NWeightedEdge<int32_t,3> edge_t;
    typedef WeightedVertex vertex_t;

    enum Type { NetworkSimplex, CostScaling, CapacityScaling, CycleCanceling };

    MinCostFlow(const BaseGraph::Ptr& graph, Type type = NetworkSimplex);

    void run();

private:
    BaseGraph::Ptr mpGraph;
    Type mType;
};


} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_MIN_COST_FLOW_HPP
