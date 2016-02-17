#ifndef GRAPH_ANALYSIS_LEMON_DIRECTED_SUB_GRAPH_HPP
#define GRAPH_ANALYSIS_LEMON_DIRECTED_SUB_GRAPH_HPP

#include <graph_analysis/SubGraphImpl.hpp>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/lemon/NodeIterator.hpp>
#include <graph_analysis/lemon/ArcIterator.hpp>

#include <lemon/adaptors.h>
#include <lemon/list_graph.h>

namespace graph_analysis {
namespace lemon {

class DirectedGraph;

typedef ::lemon::SubDigraph< ::lemon::ListDigraph, ::lemon::ListDigraph::NodeMap<bool>, ::lemon::ListDigraph::ArcMap<bool> > SubGraphLemon;

/**
 * A subgraph that provides a subset of an existing graph
 */
class DirectedSubGraph : public SubGraphImpl< SubGraphLemon, ::lemon::ListDigraph::NodeMap<bool>, ::lemon::ListDigraph::ArcMap<bool> >
{
    shared_ptr<DirectedGraph> mpDirectedGraph;
public:
    DirectedSubGraph(const shared_ptr<DirectedGraph>& graph);

    void enable(const Vertex::Ptr& vertex);
    void disable(const Vertex::Ptr& vertex);

    void enable(const Edge::Ptr& edge);
    void disable(const Edge::Ptr& edge);

    bool enabled(const Vertex::Ptr& vertex) const;
    bool enabled(const Edge::Ptr& vertex) const;
};

} // end namespace lemon
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_LEMON_DIRECTED_SUB_GRAPH_HPP
