#ifndef GRAPH_ANALYSIS_ALGORITHMS_SKIPPER_HPP

#include "../Edge.hpp"

namespace graph_analysis {
namespace algorithms {

/// Definition of the EdgeSkipper function
typedef function1<bool,Edge::Ptr> Skipper;

} // end namespace algorithms
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_ALGORITHMS_SKIPPER_HPP
