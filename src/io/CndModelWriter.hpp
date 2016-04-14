#ifndef GRAPH_ANALYSIS_IO_CND_MODEL_WRITER_HPP
#define GRAPH_ANALYSIS_IO_CND_MODEL_WRITER_HPP

#include <graph_analysis/GraphIO.hpp>

namespace graph_analysis
{
namespace io
{

/**
 * \file CndModelWriter.hpp
 * \class CndModelWriter
 * \brief Writes a graph using Elmars Component Network Description formal
 */
class CndModelWriter : public Writer
{
public:
    /**
     * \brief Stores a base graph to the provided file
     * \param filename provided output filename
     * \param graph the source graph
     */
    void write(const std::string& filename, const BaseGraph::Ptr& graph);
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_CND_MODEL_WRITER_HPP
