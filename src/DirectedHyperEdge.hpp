#ifndef GRAPH_ANALYSIS_DIRECTED_HYPER_EDGE_HPP
#define GRAPH_ANALYSIS_DIRECTED_HYPER_EDGE_HPP

#include <string>
#include "HyperEdge.hpp"

namespace graph_analysis
{

class BaseGraph;

/**
 * \brief A DirectedHyperEdge represents a set of source vertices and target
 * vertices
 * \details For the purpose of storing data in a DirectedHyperEdge, the storage type needs to be defined by inheriting from this class
 */
class DirectedHyperEdge : public HyperEdge
{
public:
    typedef shared_ptr< DirectedHyperEdge > Ptr;

    DirectedHyperEdge(const std::string& label = std::string());

    DirectedHyperEdge(const Vertex::PtrList& fromVertices,
            const Vertex::PtrList& toVertices,
            const std::string& label = std::string());

    virtual ~DirectedHyperEdge() {}

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
     * Get the source vertices
     */
    const Vertex::PtrList& getSourceVertices() const { return mSourceVertices; }

    /**
     * Get the target vertices
     */
    const Vertex::PtrList& getTargetVertices() const { return mTargetVertices; }

    /**
     * Set the source vertices
     */
    void setSourceVertices(const Vertex::PtrList& vertices) { mSourceVertices = vertices; }

    /**
     * Set the target vertices
     */
    void setTargetVertices(const Vertex::PtrList& vertices) { mTargetVertices = vertices; }
protected:
    /**
     * Get instance of an edge
     */
    virtual Vertex* getClone() const { return new DirectedHyperEdge(*this); }

private:
    Vertex::PtrList mSourceVertices;
    Vertex::PtrList mTargetVertices;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_DIRECTED_HYPER_EDGE_HPP
