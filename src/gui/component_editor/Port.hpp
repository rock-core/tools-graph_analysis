#ifndef GRAPH_ANALYSIS_PORT_HPP
#define GRAPH_ANALYSIS_PORT_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {
namespace gui {

/**
 * \brief A Port vertex inherited to allow storing data of Port type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Vertex
 */
class Port : public Vertex
{
public:
    /// invalid port id constant
    static const int INVALID_PORT_ID;

    Port(const std::string& label = std::string());
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::Port"; }

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

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PORT_HPP
