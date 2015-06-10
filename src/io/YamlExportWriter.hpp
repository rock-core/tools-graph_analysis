#ifndef GRAPH_ANALYSIS_IO_YAMLEXPORTWRITER_HPP
#define GRAPH_ANALYSIS_IO_YAMLEXPORTWRITER_HPP

#include <fstream>
#include <stdexcept>
#include <graph_analysis/GraphIO.hpp>

namespace graph_analysis {
namespace io {

class YamlExportWriter : public Writer
{
private:
    void exportVertex(std::ofstream& fout, Vertex::Ptr) const;
    void exportEdge(std::ofstream& fout, Edge::Ptr) const;
public:
    YamlExportWriter();
    ~YamlExportWriter();

    void write(const std::string& filename, const BaseGraph& graph) const;

    void write(const std::string& filename, const BaseGraph::Ptr& graph) const;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_YAMLEXPORTWRITER_HPP
