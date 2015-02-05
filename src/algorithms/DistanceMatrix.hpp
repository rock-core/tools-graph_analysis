#ifndef GRAPH_ANALYSIS_ALGORITHMS_DISTANCE_MATRIX_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_DISTANCE_MATRIX_HPP

#include <map>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {
namespace algorithms {

typedef std::map< std::pair<Vertex::Ptr, Vertex::Ptr>, double> DistanceMatrix;

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_DISTANCE_MATRIX_HPP
