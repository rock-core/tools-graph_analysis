#ifndef GRAPH_ANALYSIS_EDGE_ITERABLE_HPP
#define GRAPH_ANALYSIS_EDGE_ITERABLE_HPP

#include "EdgeIterator.hpp"

namespace graph_analysis {

class EdgeIterable
{
public:
    virtual ~EdgeIterable() {}

    /**
     * Get the node iterator
     */
    virtual EdgeIterator::Ptr getEdgeIterator() const { throw std::runtime_error("EdgeIterable::getEdgeIterator not implemented"); }

    /**
     * Get edge count
     */
    virtual uint64_t getEdgeCount() const;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_ITERABLE_HPP
