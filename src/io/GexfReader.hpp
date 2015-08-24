#ifndef GRAPH_ANALYSIS_IO_GEXF_READER_HPP
#define GRAPH_ANALYSIS_IO_GEXF_READER_HPP

#include <map>
#include <libgexf/gexf.h>
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/Edge.hpp>

namespace graph_analysis {
namespace io {

/**
 * \file GexfReader.hpp
 * \class GexfReader
 * \brief Imports a base graph from a given GEXF file
 * \details Parses an input gexf file to the requested target graph (forms a base graph)
 */
class GexfReader : public Reader
{
    typedef std::map < libgexf::t_id, Vertex::Ptr > VertexMap;
public:
    /**
     * \brief reads the graph from the given file and stores it to the provided graph argument
     * \param filename provided input filename
     * \param graph target graph to store the parsed graph
     */
    void read(const std::string& filename, BaseGraph::Ptr graph);
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GEXF_READER_HPP
