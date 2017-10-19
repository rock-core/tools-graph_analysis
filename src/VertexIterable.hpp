#ifndef GRAPH_ANALYSIS_VERTEX_ITERABLE_HPP
#define GRAPH_ANALYSIS_VERTEX_ITERABLE_HPP

#include "VertexIterator.hpp"

namespace graph_analysis {

class VertexIterable
{
public:
    virtual ~VertexIterable();

    /**
     * Get the vertex iterator
     */
    virtual VertexIterator::Ptr getVertexIterator() const { throw std::runtime_error("VertexIterable::getVertexIterator not implemented"); }

    /**
     * Get vertex count
     * Will require O(n)
     */
    virtual uint64_t getVertexCount() const;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_ITERABLE_HPP
