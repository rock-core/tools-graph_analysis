#ifndef GRAPH_ANALYSIS_ALGORITHMS_FLOYD_WARHSHALL_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_FLOYD_WARHSHALL_HPP

#include "../Edge.hpp"
#include "../BaseGraph.hpp"
#include "DistanceMatrix.hpp"

namespace graph_analysis {
namespace algorithms {

typedef function1<double, Edge::Ptr> EdgeWeightFunction;

/**
 * \brief Implements Floyd-Warshall algorithm
 * \param control exception throwing when a negative cycle is detected
 * \throw by default std::runtime_error when a negative cycle has been detected
 * \see "The Floyd–Warshall algorithm on graphs with negative cycles" (Stefan
 * Hougardy, 2010)
 */
class FloydWarshall
{
    static double getDistanceLazy(DistanceMatrix& distanceMatrix, Vertex::Ptr i, Vertex::Ptr k);
public:
    /**
     * \param graph The graph to search on
     * \param edgeWeightFunction function that allows retrieving the weight of
     * an edge
     * \param detectNegativeCycle whether to throw as soon as a negative cycle
     * has been detected
     */
    static DistanceMatrix allShortestPaths(const BaseGraph::Ptr& graph, EdgeWeightFunction edgeWeightFunction, bool detectNegativeCycle = true);
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_FLOYD_WARHSHALL_HPP
