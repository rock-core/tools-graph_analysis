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

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::Vertex"; }

    /**
     * Convert element to String
     */
    virtual std::string toString() const { return getClassName(); }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_HPP
