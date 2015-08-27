#ifndef GRAPH_ANALYSIS_ALGORITHMS_MIN_COST_FLOW_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_MIN_COST_FLOW_HPP

#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/WeightedEdge.hpp>
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
    typedef NWeightedEdge<int32_t,5> edge_t;
    typedef NWeightedVertex<int32_t,2> vertex_t;

    enum EdgeIndex { LOWER_BOUND = 0, UPPER_BOUND, COST, RESULT_FLOW };
    enum VertexIndex { SUPPLY_DEMAND = 0, RESULT_POTENTIAL };

    enum Type { NetworkSimplex, CostScaling, CapacityScaling, CycleCanceling };

    MinCostFlow(const BaseGraph::Ptr& graph, Type type = NetworkSimplex);

    /**
     * All results will be added into the graph
     * access we
     * \return the total cost
     */
    uint32_t run();

private:
    BaseGraph::Ptr mpGraph;
    Type mType;
};


} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_MIN_COST_FLOW_HPP
