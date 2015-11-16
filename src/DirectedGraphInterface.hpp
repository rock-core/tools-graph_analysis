#ifndef GRAPH_ANALYSIS_DIRECTED_GRAPH_INTERFACE_HPP
#define GRAPH_ANALYSIS_DIRECTED_GRAPH_INTERFACE_HPP

#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/EdgeIterator.hpp>

namespace graph_analysis
{

class DirectedGraphInterface : public BaseGraph
{
public:
    typedef shared_ptr<DirectedGraphInterface> Ptr;

    /**
     * Default constructor
     */
    DirectedGraphInterface(BaseGraph::ImplementationType type)
        : BaseGraph(type)
    {}

    virtual ~DirectedGraphInterface() {}

    virtual EdgeIterator::Ptr getOutEdgeIterator(const Vertex::Ptr& vertex) const = 0;
    virtual EdgeIterator::Ptr getInEdgeIterator(const Vertex::Ptr& vertex) const = 0;

    SpecializedIterable<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr> inEdges(const Vertex::Ptr& vertex) const;
    SpecializedIterable<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr> outEdges(const Vertex::Ptr& vertex) const;
};

} // end graph_analysis
#endif // GRAPH_ANALYSIS_DIRECTED_GRAPH_INTERFACE_HPP
