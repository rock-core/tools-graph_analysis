#ifndef GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP
#define GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP

#include <graph_analysis/GraphIO.hpp>

namespace graph_analysis {
namespace io {

/**
 * \file GraphvizWriter.hpp
 * \class GraphvizWriter
 * \brief graphviz based graph renderer
 * \details Exports a given base graph to a given dot file using GraphViz renderToFile() interface
 */
class GraphvizWriter : public Writer
{
    /// layouting engine to re-arrange the graph through GraphViz rendering
    std::string mLayout;
public:
    /// constructor
    GraphvizWriter(const std::string& layout = "dot");
    /// destructor
    ~GraphvizWriter();

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
#endif // GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP
