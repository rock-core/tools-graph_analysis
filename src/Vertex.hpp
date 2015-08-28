#ifndef GRAPH_ANALYSIS_VERTEX_HPP
#define GRAPH_ANALYSIS_VERTEX_HPP

#include <iostream>
#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis
{

/**
 * \brief A vertex inherited to allow storing data of any type
 * \details For the purpose of storing data with a vertex, the storage type needs to inherit
 * from this class
 */
class Vertex : public GraphElement
{
public:
    typedef boost::shared_ptr< Vertex > Ptr;

    virtual ~Vertex() {}

    Vertex(const std::string& label = std::string());

    /**
     * Clone this vertex -- the returned vertex will have no
     * graph association
     */
    Vertex::Ptr clone() const;

    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::Vertex"; }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::Vertex"; }

    /**
     * Convert element to string
     */
    virtual std::string toString() const;


protected:
    /**
     * Create a copy of this vertex
     */
    virtual Vertex* getClone() const { return new Vertex(*this); }

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_HPP
