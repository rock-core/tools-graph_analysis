#ifndef GRAPH_ANALYSIS_IO_CND_MODEL_READER_HPP
#define GRAPH_ANALYSIS_IO_CND_MODEL_READER_HPP

#include <graph_analysis/GraphIO.hpp>

namespace graph_analysis
{
namespace io
{

/**
 * \file CndModelReader.hpp
 * \class CndModelReader
 * \brief Parses an inputfile with Elmars Component Network Description
 */
class CndModelReader : public Reader
{
public:
    /**
     * \brief reads the graph from the given file and stores it to the provided
     * graph argument
     * \param filename provided input filename
     * \param graph target graph to store the parsed graph
     */
    void read(const std::string& filename, BaseGraph::Ptr graph);
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_CND_MODEL_READER_HPP
