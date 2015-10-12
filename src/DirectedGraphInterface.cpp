#include "DirectedGraphInterface.hpp"
#include "EdgeIterator.hpp"
#include "Vertex.hpp"

using namespace graph_analysis;


SpecializedIterable<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr> DirectedGraphInterface::inEdges(const Vertex::Ptr& vertex) const
{
    return SpecializedIterable<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr>(this, vertex, &DirectedGraphInterface::getInEdgeIterator);
}

SpecializedIterable<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr> DirectedGraphInterface::outEdges(const Vertex::Ptr& vertex) const
{
    return SpecializedIterable<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr>(this, vertex, &DirectedGraphInterface::getOutEdgeIterator);
}
