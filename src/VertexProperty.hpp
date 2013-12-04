#ifndef GRAPH_ANALYSIS_VERTEX_PROPERTY_HPP
#define GRAPH_ANALYSIS_VERTEX_PROPERTY_HPP

#include <boost/shared_ptr.hpp>

namespace graph_analysis
{

/**
 * \brief A VertexProperty can be associated with a vertex to store additional data.
 * \details For the purpose of storing data with a vertex, the storage type needs to inherit
 * from this class
 * \tparam VertexType The vertex type the property will be associated with
 */
template<typename VertexType>
class VertexProperty
{
public:
    typedef boost::shared_ptr< VertexProperty<VertexType> > Ptr;

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::VertexProperty"; }

    /**
     * Set the corresponding vertex to allow reverse mapping from the property to the
     * vertex
     * \param vertex The vertex that this property is associated with
     */
    void setVertex(const VertexType& vertex) { mVertex = vertex; }

    /**
     * Get vertex
     * \return vertex
     */
    VertexType getVertex() { return mVertex; }

private:
    VertexType mVertex;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_PROPERTY_HPP
