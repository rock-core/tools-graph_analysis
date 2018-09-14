#ifndef GRAPH_ANALYSIS_SNAP_DIRECTED_SUB_GRAPH_HPP
#define GRAPH_ANALYSIS_SNAP_DIRECTED_SUB_GRAPH_HPP

#include "../SubGraph.hpp"

namespace graph_analysis {
namespace snap {

class DirectedGraph;

/**
 * Since SNAP does not natively provide a subgraph implementation
 * we have to maintain the subgraph explicitly
 */
class DirectedSubGraph : public SubGraph
{
public:
    DirectedSubGraph(const shared_ptr<DirectedGraph>& graph);
};

} // end namespace snap
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SNAP_DIRECTED_SUB_GRAPH_HPP
