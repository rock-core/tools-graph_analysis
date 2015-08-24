#ifndef GRAPH_ANALYSIS_IO_GEXF_WRITER_HPP
#define GRAPH_ANALYSIS_IO_GEXF_WRITER_HPP

#include <graph_analysis/GraphIO.hpp>

namespace graph_analysis {
namespace io {

/**
 * \file GexfWriter.hpp
 * \class GexfWriter
 * \brief exports a given base graph to a given file in GEXF format
 * \details Renders requested graph to GEXF standard format
 */
class GexfWriter : public Writer
{
public:
    /**
     * \brief outputs the given graph to the given file
     * \param filename requested output filename
     * \param graph requested graph to be printed
     */
    void write(const std::string& filename, const BaseGraph& graph) const;
    /**
     * \brief outputs the given graph to the given file
     * \param filename requested output filename
     * \param graph smart pointer to the requested graph to be printed
     */
    void write(const std::string& filename, const BaseGraph::Ptr& graph) const;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GEXF_WRITER_HPP

