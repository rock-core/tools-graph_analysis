#ifndef GRAPH_ANALYSIS_IO_YAMLEXPORTWRITER_HPP
#define GRAPH_ANALYSIS_IO_YAMLEXPORTWRITER_HPP

#include <string>
#include <fstream>
#include <stdexcept>
#include <graph_analysis/GraphIO.hpp>

namespace graph_analysis {
namespace io {

/**
 * \class YamlWriter
 * \author Clasian
 * \file YamlWriter.hpp
 * \brief Renders requested graph to custom YAML/yml format
 */
class YamlWriter : public Writer
{
private:
    /**
     * \brief prints to given stream the requested vertex of the given graph
     * \param graph the given graph to render
     * \param fout the given stream to output to
     * \param vertex the requested vertex to render
     */
    void exportVertex(const BaseGraph::Ptr& graph, std::ofstream& fout, Vertex::Ptr vertex) const;
    /**
     * \brief prints to given stream the requested edge of the given graph
     * \param graph the given graph to render
     * \param fout the given stream to output to
     * @param edge the requested edge to render
     */
    void exportEdge(const BaseGraph::Ptr& graph, std::ofstream& fout, Edge::Ptr edge) const;
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
#endif // GRAPH_ANALYSIS_IO_YAMLEXPORTWRITER_HPP
