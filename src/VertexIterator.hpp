#ifndef GRAPH_ANALYSIS_VERTEX_ITERATOR_HPP
#define GRAPH_ANALYSIS_VERTEX_ITERATOR_HPP

#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {

/**
 * \brief VertexIterator interface
 * Allows to access all vertices of a graph. Requires implementation
 * to wrap the underlyning graph library in use
 */
class VertexIterator
{
public:
    typedef boost::shared_ptr<VertexIterator> Ptr;

    /**
     * \brief Test if there is a next item and if it is then updates
     * the current to next
     * \throws std::runtime_error if not implemented by subclass
     */
    virtual bool next() { return false; }

    /**
     * Retrieve the current vertex this iterator is pointing to
     */
    Vertex::Ptr current() { return mVertex; }

protected:
    /**
     * \brief Set next vertex
     * Should be applied by class implementing next()
     */
    void setNext(Vertex::Ptr vertex) { mVertex = vertex; }

private:
    Vertex::Ptr mVertex;
};

class VertexIterable
{
public:
    /**
     * Get the vertex iterator
     */
    virtual VertexIterator::Ptr getVertexIterator() = 0;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_ITERATOR_HPP
