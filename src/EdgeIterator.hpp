#ifndef GRAPH_ANALYSIS_EDGE_ITERATOR_HPP
#define GRAPH_ANALYSIS_EDGE_ITERATOR_HPP

#include <graph_analysis/Edge.hpp>

namespace graph_analysis {

/**
 * \brief EdgeIterator interface
 * Allows to access all edges of a graph. Requires implementation
 * to wrap the underlying graph library in use
 */
class EdgeIterator
{
public:
    typedef boost::shared_ptr<EdgeIterator> Ptr;

    /**
     * \brief Test if there is a next item and if it is then updates
     * the current to next
     * \throws std::runtime_error if not implemented by subclass
     */
    virtual bool next() { throw std::runtime_error("EdgeIterator: next() has not been implemented"); }

    /**
     * Retrieve the current edge this iterator is pointing to
     */
    Edge::Ptr current() { return mEdge; }

protected:
    /**
     * \brief Set next edge
     * Should be applied by class implementing next()
     */
    void setNext(Edge::Ptr edge) { mEdge = edge; }

private:
    Edge::Ptr mEdge;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_ITERATOR_HPP
