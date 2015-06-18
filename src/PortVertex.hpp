#ifndef GRAPH_ANALYSIS_PORTVERTEX_HPP
#define GRAPH_ANALYSIS_PORTVERTEX_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis
{

/**
 * \brief A Port vertex inherited to allow storing data of Port type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Vertex
 */
class PortVertex : public Vertex
{
public:
    PortVertex(const std::string& label = "");
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::PortVertex"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual PortVertex* getClone() const { return new PortVertex(*this); }

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PORTVERTEX_HPP
