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

    virtual ~Vertex() {}

    Vertex(const std::string& label = "");

    typedef boost::shared_ptr< Vertex > Ptr;

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::Vertex"; }

    /**
     * Convert element to string
     */
    std::string toString() const;

protected:
    std::string mLabel;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_HPP
