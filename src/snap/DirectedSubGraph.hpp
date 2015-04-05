#ifndef GRAPH_ANALYSIS_SNAP_DIRECTED_SUB_GRAPH_HPP
#define GRAPH_ANALYSIS_SNAP_DIRECTED_SUB_GRAPH_HPP

#include <graph_analysis/SubGraph.hpp>

namespace graph_analysis {

// Forward declaration
class BaseGraph;

namespace snap {

class DirectedSubGraph : public SubGraph
{
    boost::shared_ptr<BaseGraph> mDirectedGraph;

public:
    DirectedSubGraph(boost::shared_ptr<BaseGraph> graph, boost::shared_ptr<BaseGraph> subGraph);

    void enable(Vertex::Ptr vertex);
    void disable(Vertex::Ptr vertex);

    void enable(Edge::Ptr edge);
    void disable(Edge::Ptr edge);

    bool enabled(Vertex::Ptr vertex) const;
    bool enabled(Edge::Ptr edge) const;
};

} // end namespace snap
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SNAP_DIRECTED_SUB_GRAPH_HPP
