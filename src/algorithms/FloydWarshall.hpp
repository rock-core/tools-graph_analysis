#ifndef GRAPH_ANALYSIS_ALGORITHMS_FLOYD_WARHSHALL_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_FLOYD_WARHSHALL_HPP

#include <boost/function.hpp>
#include <graph_analysis/Edge.hpp>
#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/algorithms/DistanceMatrix.hpp>

namespace graph_analysis {
namespace algorithms {

typedef boost::function1<double, Edge::Ptr> EdgeWeightFunction;

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
    static DistanceMatrix allShortestPaths(BaseGraph::Ptr graph, EdgeWeightFunction edgeWeightFunction, bool detectNegativeCycle = true);
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_FLOYD_WARHSHALL_HPP
