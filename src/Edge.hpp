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
    typedef boost::shared_ptr< Edge > Ptr;

    Edge(const std::string& label = "");

    Edge(Vertex::Ptr source, Vertex::Ptr target, const std::string& label = "");

    virtual ~Edge() {}

    /**
     * Clone this edge -- the returned edge will have no
     * graph association
     */
    Edge::Ptr clone() const;

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

    /**
     * Get all involved vertices for two edges
     * \return all distinct vertices of two edges
     */
    static std::vector<Vertex::Ptr> getInvolvedVertices(Edge::Ptr e0, Edge::Ptr e1);

    /**
     * Check if two edges meet at a vertex
     * \return true if edges meet at least one vertex
     */
    static bool areMeeting(Edge::Ptr e0, Edge::Ptr e1);

    /**
     * Get label
     */ 
    std::string getLabel() const { return mLabel; }
    /**
     * Get the label from before
     */ 
    std::string getCachedLabel() const { return mCachedLabel; }
    void setLabel(const std::string &label) { mCachedLabel = mLabel; mLabel = label; }
    
    /**
     * Revert edge label contents
     */
    void revertLabel() { mLabel = mCachedLabel; }

    void unSetActive() { mBidirectionalActive = false; }
    void setActive() { mBidirectionalActive = true; }
    bool isActive() { return mBidirectionalActive; }

protected:
    /**
     * Get instance of an edge
     */
    virtual Edge* getClone() const { return new Edge(*this); }

private:
    Vertex::Ptr mSourceVertex;
    Vertex::Ptr mTargetVertex;
    std::string mCachedLabel;
    std::string mLabel;
    bool mBidirectionalActive;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_HPP
