#ifndef GRAPH_ANALYSIS_EDGE_HPP
#define GRAPH_ANALYSIS_EDGE_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {

/**
 * \brief An Edge represents the link between two vertices
 * \details For the purpose of storing data in a edge, the storage type needs to be defined by inheriting
 * from this class, some default implementations exists
 */
class Edge : public GraphElement
{
public:
    Edge(const std::string& label = "");
    Edge(Vertex::Ptr source, Vertex::Ptr target, const std::string& label = "");

    virtual ~Edge() {}

    typedef boost::shared_ptr< Edge > Ptr;

    // Get class name
    // \return class name
    virtual std::string getClassName() const;

    /**
     * Convert element to string
     * \return string representation of edge
     */
    virtual std::string toString() const;

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

    /// setter method for mLabel
    void setLabel(const std::string &label) { mLabel = label; }
    /// getter method for mLabel
    std::string getLabel() const { return mLabel; }

private:
    Vertex::Ptr mSourceVertex;
    Vertex::Ptr mTargetVertex;
    std::string mLabel;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_HPP
