#ifndef GRAPH_ANALYSIS_BASEVERTEX_HPP
#define GRAPH_ANALYSIS_BASEVERTEX_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis
{

/**
 * \brief A Base vertex inherited to allow storing data of Base type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Vertex
 */
class BaseVertex : public Vertex
{
public:
    BaseVertex() {}
    BaseVertex(const std::string& label = "");
    std::string type() { return "base"; }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_BASEVERTEX_HPP
