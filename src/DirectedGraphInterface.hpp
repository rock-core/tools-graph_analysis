#ifndef GRAPH_ANALYSIS_DIRECTED_GRAPH_INTERFACE_HPP
#define GRAPH_ANALYSIS_DIRECTED_GRAPH_INTERFACE_HPP

#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/EdgeIterator.hpp>

namespace graph_analysis
{

class DirectedGraphInterface : public BaseGraph
{
public:
    typedef boost::shared_ptr<DirectedGraphInterface> Ptr;

    /**
     * Default constructor
     */
    DirectedGraphInterface(BaseGraph::ImplementationType type)
        : BaseGraph(type)
    {}

    virtual ~DirectedGraphInterface() {}

    virtual EdgeIterator::Ptr getOutEdgeIterator(Vertex::Ptr vertex) const = 0;
    virtual EdgeIterator::Ptr getInEdgeIterator(Vertex::Ptr vertex) const = 0;

    SpecializedIterator<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr> inEdges(Vertex::Ptr vertex) const;
    SpecializedIterator<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr> outEdges(Vertex::Ptr vertex) const;

};

} // end graph_analysis
#endif // GRAPH_ANALYSIS_DIRECTED_GRAPH_INTERFACE_HPP
