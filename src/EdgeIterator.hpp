#ifndef GRAPH_ANALYSIS_EDGE_ITERATOR_HPP
#define GRAPH_ANALYSIS_EDGE_ITERATOR_HPP

#include "SharedPtr.hpp"
#include "Edge.hpp"

namespace graph_analysis {

/**
 * \brief EdgeIterator interface
 * Allows to access all edges of a graph. Requires implementation
 * to wrap the underlying graph library in use
 */
class EdgeIterator
{
    // Allow for usage of skip function
    friend class SubGraph;

public:
    typedef shared_ptr<EdgeIterator> Ptr;
    typedef function1<bool, const Edge::Ptr&> SkipFunction;

    virtual ~EdgeIterator() {}

    /**
     * \brief Test if there is a next item and if it is then updates
     * the current to next
     * \throws std::runtime_error if not implemented by subclass
     */
    virtual bool next() { throw std::runtime_error("EdgeIterator: next() has not been implemented"); }

    /**
     * Retrieve the current edge this iterator is pointing to
     */
    const Edge::Ptr& current() { return mEdge; }

protected:
    /**
     * \brief Check if edge should be skipped
     * \return True if edge should be skipped, false otherwise
     */
    bool skip(const Edge::Ptr& edge) const;

    /**
     * \brief Set next edge
     * Should be applied by class implementing next()
     */
    void setNext(const Edge::Ptr& edge) { mEdge = edge; }

    /**
     * Allows to add a skip function -- e.g. for the SubGraph which
     * provides a 'view' onto the BaseGraph
     */
    void setSkipFunction(SkipFunction skipFunction) { mSkipFunction = skipFunction; }

private:
    Edge::Ptr mEdge;
    SkipFunction mSkipFunction;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_ITERATOR_HPP
