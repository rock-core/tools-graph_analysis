#ifndef GRAPH_ANALYSIS_PORT_HPP
#define GRAPH_ANALYSIS_PORT_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {

/**
 * \brief A Port vertex inherited to allow storing data of Port type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Vertex
 */
class Port : public Vertex
{
public:
    typedef shared_ptr< Port > Ptr;
    Port(const std::string& label = "");
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return vertexType(); }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::Port"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual Port* getClone() const { return new Port(*this); }

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PORT_HPP
