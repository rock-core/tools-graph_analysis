#ifndef GRAPH_ANALYSIS_EDGE_HPP
#define GRAPH_ANALYSIS_EDGE_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {

/**
 * \brief A EdgeProperty can be associated with a edge to store additional data.
 * \details For the purpose of storing data with a edge, the storage type needs to inherit
 * from this class
 * \tparam GraphType The GraphType the edge belongs to
 * \tparam EdgeType The edge type the property will be associated with
 */
class Edge : public GraphElement
{
public:
    Edge()
        : GraphElement()
    {}

    Edge(Vertex::Ptr source, Vertex::Ptr target)
        : mSourceVertex(source)
        , mTargetVertex(target)
    {}

    typedef boost::shared_ptr< Edge > Ptr;

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::EdgeProperty"; }

    /**
     * Convert element to String
     */
    virtual std::string toString() const { return getClassName(); }

    /**
     * Get the source vertex
     */
    Vertex::Ptr getSourceVertex() const { return mSourceVertex; }

    /**
     * Get the target vertex
     */
    Vertex::Ptr getTargetVertex() const { return mTargetVertex; }

    /**
     * Set the source vertex
     */
    void setSourceVertex(Vertex::Ptr source) { mSourceVertex = source; }

    /**
     * Set the target vertex
     */
    void setTargetVertex(Vertex::Ptr target) { mTargetVertex = target; }

private:
    Vertex::Ptr mSourceVertex;
    Vertex::Ptr mTargetVertex;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_PROPERTY_HPP
