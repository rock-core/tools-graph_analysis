#ifndef GRAPH_ANALYSIS_IO_YAML_READER_HPP
#define GRAPH_ANALYSIS_IO_YAML_READER_HPP

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/Edge.hpp>

namespace graph_analysis {
namespace io {

/**
 * \class YamlReader
 * \file YamlReader.hpp
 * \brief Parses an inputfile with custom YAML/yml format to requested graph (i.e. custom yml that simply lists the nodes with their properties and then lists edges and their properties)
 */
class YamlReader : public Reader
{
    typedef std::map < std::string, Vertex::Ptr > VertexMap;
private:
    /**
     * \brief parses all nodes from the provided string
     * \param nodeStream input string containing all the nodes already tokenized out of the inputfile
     * \param graph target graph where to add the parsed nodes to
     * \param vMap map to store all nodes indexed by their ID as map key
     */
    void parseNodes(std::stringstream& nodeStream, const BaseGraph::Ptr& graph, VertexMap& vMap) const;
    /**
     * \brief parses all edges from the provided string
     * \param edgeStream input string containing all the edges already tokenized out of the inputfile
     * \param graph target graph where to add the parsed edges to
     * \param vMap map that already stores all nodes indexed by their ID as map key
     */
    void parseEdges(std::stringstream& edgeStream, const BaseGraph::Ptr& graph, VertexMap& vMap) const;
    /**
     * \brief parses a single property (i.e. 1 line)
     * \param keyword expected beginning token
     * \param stream given string to read from
     * \param graph target graph
     * \return parsed property value
     */
    std::string nextToken(const std::string& keyword, std::stringstream& stream) const;
    /// throws on reason: "keyword" was expected -> "word" was encountered
    void die(const std::string& keyword, const std::string& word) const;
    /// throws on provided "msg" reason
    void die(const std::string& msg) const;
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
#endif // GRAPH_ANALYSIS_IO_YAML_READER_HPP
