#ifndef GRAPH_ANALYSIS_ALGORITHMS_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_HPP

#include "SharedPtr.hpp"

namespace graph_analysis {

class SubGraph;
class BaseGraph;

/**
  * \brief Interface definition for a collection of standard algorithms
  */
class Algorithms
{
public:
    virtual ~Algorithms() {}

    /**
      * Identify the set of connected components
      * \return SubGraph with the set of vertices, where each vertex is a member
      * of a single component, i.e.,
      * the number of vertices in the subgraph defines the number of found
      * components, and the vertices which are connected to each of
      * these vertices define a component
      */
    virtual shared_ptr<SubGraph> identifyConnectedComponents(const shared_ptr<BaseGraph>& baseGraph) const { (void) baseGraph; throw std::runtime_error("graph_analysis::Algorithms::identifyConnectedComponents has not been implemented"); }

    /**
      * Test if a graph is connected
      */
    virtual bool isConnected() const { throw std::runtime_error("graph_analysis::Algorithms::isConnected has not been implemented"); }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_HPP
