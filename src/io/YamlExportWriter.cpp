#include "YamlExportWriter.hpp"
#include <base/Logging.hpp>
#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis {
namespace io {

YamlExportWriter::YamlExportWriter()
{}

YamlExportWriter::~YamlExportWriter()
{}

void YamlExportWriter::write(const std::string& filename, const BaseGraph::Ptr& graph) const
{
    const char *fname = filename.c_str();
    std::ofstream fout(fname);
    if(!fout.is_open() || fout.fail())
    {
        std::string error_msg = std::string("graph_analysis::io::YamlExportWriter: failed to open file ") + filename + " for graph export";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    LOG_INFO("graph_analysis::io::YamlExportWriter: rendering graph to file \"%s\" ", fname);

    VertexIterator::Ptr nodeIt = graph->getVertexIterator();
    fout << "nodes:\n";
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();
        exportVertex(fout, vertex);
    }

    EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
    fout << "edges:\n";
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        exportEdge(fout, edge);
    }
    if(fout.is_open())
    {
        fout.close();
    }
}

void YamlExportWriter::exportVertex(std::ofstream& fout, Vertex::Ptr vertex) const
{
    fout << "  - id: " << vertex->getUid() << '\n';
    fout << "    label: " << vertex->getLabel() << '\n';
    fout << "    type: " << vertex->getClassName() << '\n';
}

void YamlExportWriter::exportEdge(std::ofstream& fout, Edge::Ptr edge) const
{
    fout << "  - fromNodeId: " << edge->getSourceVertex()->getUid() << '\n';
    fout << "    toNodeId: " << edge->getTargetVertex()->getUid() << '\n';
    fout << "    label: " << edge->getLabel() << '\n';
}

} // end namespace io
} // end namespace graph_analysis
