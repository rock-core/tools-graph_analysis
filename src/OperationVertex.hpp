#ifndef GRAPH_ANALYSIS_OPERATIONVERTEX_HPP
#define GRAPH_ANALYSIS_OPERATIONVERTEX_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis
{

/**
 * \brief An Operation vertex inherited to allow storing data of Operation type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Vertex
 */
class OperationVertex : public Vertex
{
public:
    OperationVertex(const std::string& label = std::string());
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::OperationVertex"; }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::OperationVertex"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual OperationVertex* getClone() const { return new OperationVertex(*this); }

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_OPERATIONVERTEX_HPP
