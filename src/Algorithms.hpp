#ifndef GRAPH_ANALYSIS_ALGORITHMS_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_HPP

#include <boost/shared_ptr.hpp>

namespace graph_analysis {

class SubGraph;

class Algorithms
{
public:
    virtual boost::shared_ptr<SubGraph> identifyConnectedComponents() { throw std::runtime_error("graph_analysis::Algorithms::identifyConnectedComponents has not been implemented"); }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_HPP
