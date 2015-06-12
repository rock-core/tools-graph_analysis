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

class YamlReader : public Reader
{
    typedef std::map < std::string, Vertex::Ptr > VertexMap;
private:
    void parseNodes(std::stringstream& nodeStream, const BaseGraph::Ptr& graph, VertexMap& vMap) const;
    void parseEdges(std::stringstream& edgeStream, const BaseGraph::Ptr& graph, VertexMap& vMap) const;
    std::string nextToken(const std::string& keyword, std::stringstream& stream, const BaseGraph::Ptr& graph) const;
    void die(const std::string& keyword, const std::string& word) const;
    void die(const std::string& msg) const;
public:
    void read(const std::string& filename, const BaseGraph::Ptr& graph) const;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_YAML_READER_HPP
