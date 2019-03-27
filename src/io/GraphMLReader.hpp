#ifndef GRAPH_ANALYSIS_IO_GRAPHML_READER_HPP
#define GRAPH_ANALYSIS_IO_GRAPHML_READER_HPP

#include <map>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "../GraphIO.hpp"
#include "../Vertex.hpp"
#include "../Edge.hpp"

namespace graph_analysis {
namespace io {

/**
 * \file GraphMLReader.hpp
 * \class GraphMLReader
 * \brief Imports a base graph from a given GRAPHML file
 * \details Parses an input gexf file to the requested target graph (forms a base graph)
 */
class GraphMLReader : public Reader
{
    typedef std::map < std::string, Vertex::Ptr > VertexMap;
    struct GraphMLNode
    {
        Vertex::Ptr vertex;
        std::string id;
    };
    struct GraphMLEdge
    {
        Edge::Ptr edge;
        std::string id;
        std::string source;
        std::string target;
    };

public:
    /**
     * \brief reads the graph from the given file and stores it to the provided graph argument
     * \param filename provided input filename
     * \param graph target graph to store the parsed graph
     */
    void read(const std::string& filename, BaseGraph::Ptr graph);

    GraphMLNode getNode(xmlNodePtr node, xmlXPathContextPtr context) const;
    GraphMLEdge getEdge(xmlNodePtr node, xmlXPathContextPtr context) const;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GRAPHML_READER_HPP
