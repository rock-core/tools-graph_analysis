#ifndef GRAPH_ANALYSIS_OPERATION_HPP
#define GRAPH_ANALYSIS_OPERATION_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {
namespace gui {

/**
 * \brief An Operation vertex inherited to allow storing data of Operation type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Vertex
 */
class Operation : public Vertex
{
public:
    Operation(const std::string& label = std::string());
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::Operation"; }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::Operation"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual Operation* getClone() const { return new Operation(*this); }

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_OPERATION_HPP
