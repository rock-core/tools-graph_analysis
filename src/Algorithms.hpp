#ifndef GRAPH_ANALYSIS_ALGORITHMS_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_HPP

#include "SharedPtr.hpp"

namespace graph_analysis {

class SubGraph;
class BaseGraph;

class Algorithms
{
public:
    virtual ~Algorithms() {}

    virtual shared_ptr<SubGraph> identifyConnectedComponents(const shared_ptr<BaseGraph>& baseGraph) const { (void) baseGraph; throw std::runtime_error("graph_analysis::Algorithms::identifyConnectedComponents has not been implemented"); }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_HPP
