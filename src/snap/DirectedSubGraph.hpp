#ifndef GRAPH_ANALYSIS_SNAP_DIRECTED_SUB_GRAPH_HPP
#define GRAPH_ANALYSIS_SNAP_DIRECTED_SUB_GRAPH_HPP

#include <graph_analysis/SubGraph.hpp>

namespace graph_analysis {

// Forward declaration
class BaseGraph;

namespace snap {

/**
 * Since SNAP does not natively provide a subgraph implementation
 * we have to maintain the subgraph explicitely
 */
class DirectedSubGraph : public SubGraph
{
    /// Maintain the subgraph structure in here
    shared_ptr<BaseGraph> mDirectedGraph;

public:
    DirectedSubGraph(const shared_ptr<BaseGraph>& graph);

    void enable(const Vertex::Ptr& vertex);
    void disable(const Vertex::Ptr& vertex);

    void enable(const Edge::Ptr& edge);
    void disable(const Edge::Ptr& edge);

    bool enabled(const Vertex::Ptr& vertex) const;
    bool enabled(const Edge::Ptr& edge) const;
};

} // end namespace snap
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SNAP_DIRECTED_SUB_GRAPH_HPP
