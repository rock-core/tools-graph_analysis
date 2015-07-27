#ifndef GRAPH_ANALYSIS_OUTPUTPORTVERTEX_HPP
#define GRAPH_ANALYSIS_OUTPUTPORTVERTEX_HPP

#include <iostream>
#include <graph_analysis/PortVertex.hpp>
#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis
{

/**
 * \brief An OutputPort vertex inherited to allow storing data of output Port type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class PortVertex
 */
class OutputPortVertex : public PortVertex
{
public:
    OutputPortVertex(const std::string& label = "");
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::OutputPortVertex"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual OutputPortVertex* getClone() const { return new OutputPortVertex(*this); }

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_OUTPUTPORTVERTEX_HPP
