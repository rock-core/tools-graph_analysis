#ifndef GRAPH_ANALYSIS_HYPER_EDGE_HPP
#define GRAPH_ANALYSIS_HYPER_EDGE_HPP

#include <string>
#include "Vertex.hpp"

namespace graph_analysis
{

class BaseGraph;

/**
 * \brief A HyperEdge represents a set of vertices
 * \details For the purpose of storing data in a HyperEdge, the storage type needs to be defined by inheriting
 * from this class
 */
class HyperEdge : public Vertex
{
public:
    typedef shared_ptr< HyperEdge > Ptr;

    HyperEdge(const std::string& label = std::string());

    HyperEdge(const Vertex::PtrList& vertices, const std::string& label = std::string());

    virtual ~HyperEdge() {}

    // Get class name
    // \return class name
    virtual std::string getClassName() const;

    /**
     * Convert element to string
     * \return string representation of edge
     */
    virtual std::string toString() const;

    /**
     * Convert element to string
     * \return string representation of edge
     */
    virtual std::string toString(uint32_t indent) const;

    /**
     * Get the vertices
     */
    const Vertex::PtrList& getVertices() const { return mVertices; }

    /**
     * Set the source vertex
     */
    void setVertices(const Vertex::PtrList& vertices) { mVertices = vertices; }

    /**
     * Get all involved vertices for two edges
     * \return all distinct vertices of two edges
     */
    static std::vector<Vertex::Ptr> getInvolvedVertices(const HyperEdge::Ptr& e0, const HyperEdge::Ptr& e1);

    /**
     * Check if two HyperEdges meet at a particular vertex
     * \return true if edges meet at least one vertex
     */
    static bool areMeeting(const HyperEdge::Ptr& e0, const HyperEdge::Ptr& e1);

protected:
    /**
     * Get instance of an edge
     */
    virtual Vertex* getClone() const { return new HyperEdge(*this); }

private:
    Vertex::PtrList mVertices;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_HYPER_EDGE_HPP
