#ifndef GRAPH_ANALYSIS_BOOST_DIRECTED_SUB_GRAPH_HPP
#define GRAPH_ANALYSIS_BOOST_DIRECTED_SUB_GRAPH_HPP

#include "../SubGraphImpl.hpp"

namespace graph_analysis {
namespace boost_graph {

class DirectedGraph;

/**
 * A subgraph that provides a subset of an existing graph
 */
class DirectedSubGraph : public SubGraph
{
public:
    DirectedSubGraph(const shared_ptr<DirectedGraph>& graph);
};

} // end namespace lemon
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_BOOST_DIRECTED_SUB_GRAPH_HPP
