#include "DirectedSubGraph.hpp"
#include "DirectedGraph.hpp"

namespace graph_analysis {
namespace boost_graph {

DirectedSubGraph::DirectedSubGraph(const shared_ptr<DirectedGraph>& graph)
    : SubGraph(graph)
{}

} // end namespace boost_graph
} // end namespace graph_analysis
