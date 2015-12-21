#ifndef GRAPH_ANALYSIS_VERTEX_ITERATOR_HPP
#define GRAPH_ANALYSIS_VERTEX_ITERATOR_HPP

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/SharedPtr.hpp>


namespace graph_analysis {

/**
 * \brief VertexIterator interface
 * Allows to access all vertices of a graph. Requires implementation
 * to wrap the underlyning graph library in use
 */
class VertexIterator
{
    // Allow for usage of skip function
    friend class SubGraph;

public:
    typedef shared_ptr<VertexIterator> Ptr;
    typedef function1<bool, const Vertex::Ptr&> SkipFunction;

    virtual ~VertexIterator() {}

    /**
     * \brief Test if there is a next item and if it is then updates
     * the current to next
     * \throws std::runtime_error if not implemented by subclass
     */
    virtual bool next() { return false; }

    /**
     * Retrieve the current vertex this iterator is pointing to
     */
    const Vertex::Ptr& current() { return mVertex; }

protected:
    /**
     * Check if vertex should be skipped
     * \return True if vertex should be skipped, false otherwise
     */
    bool skip(const Vertex::Ptr& vertex) const;

    /**
     * \brief Set next vertex
     * Should be applied by class implementing next()
     */
    void setNext(const Vertex::Ptr& vertex) { mVertex = vertex; }

    /**
     * Allows to set a skip function
     */
    void setSkipFunction(SkipFunction skipFunction) { mSkipFunction = skipFunction; }

private:
    Vertex::Ptr mVertex;
    SkipFunction mSkipFunction;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_ITERATOR_HPP
