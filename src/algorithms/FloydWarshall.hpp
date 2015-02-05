#ifndef GRAPH_ANALYSIS_ALGORITHMS_FLOYD_WARHSHALL_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_FLOYD_WARHSHALL_HPP

#include <boost/function.hpp>
#include <graph_analysis/Edge.hpp>
#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/algorithms/DistanceMatrix.hpp>

namespace graph_analysis {
namespace algorithms {

typedef boost::function1<double, Edge::Ptr> EdgeWeightFunction;

class FloydWarshall
{
    static double getDistanceLazy(DistanceMatrix& distanceMatrix, Vertex::Ptr i, Vertex::Ptr k);
public:
    static DistanceMatrix allShortestPath(BaseGraph::Ptr graph, EdgeWeightFunction edgeWeightFunction);
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_FLOYD_WARHSHALL_HPP
