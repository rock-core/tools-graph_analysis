#ifndef GRAPH_ANALYSIS_IO_GRAPHML_WRITER_HPP
#define GRAPH_ANALYSIS_IO_GRAPHML_WRITER_HPP

#include "../GraphIO.hpp"

namespace graph_analysis {
namespace io {

/**
 * \file GraphMLWriter.hpp
 * \class GraphMLWriter
 * \brief exports a given base graph to a given file in GRAPHML format
 * \details Renders requested graph to GRAPHML standard format
 */
class GraphMLWriter : public Writer
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

    /**
     * Create a list of nodes associated with a graph
     */
    std::string generateKeyList(const std::set<std::string>& vertexTypes, const std::set<std::string>& edgeTypes, size_t indent) const;
    std::string generateNodeList(const BaseGraph::Ptr& graph, size_t indent, std::set<std::string>& vertexTypes) const;
    std::string generateEdgeList(const BaseGraph::Ptr& graph, size_t indent, std::set<std::string>& edgeTypes) const;
};
} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GRAPHML_WRITER_HPP
