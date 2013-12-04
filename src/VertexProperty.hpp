#ifndef GRAPH_ANALYSIS_VERTEX_PROPERTY_HPP
#define GRAPH_ANALYSIS_VERTEX_PROPERTY_HPP

#include <boost/shared_ptr.hpp>

namespace graph_analysis
{

template<typename VertexType>
class VertexProperty
{
public:
    typedef boost::shared_ptr< VertexProperty<VertexType> > Ptr;

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::VertexProperty"; }

    /**
     * Set the corresponding vertex
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
