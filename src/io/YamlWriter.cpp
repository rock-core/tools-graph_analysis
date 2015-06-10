#include "YamlWriter.hpp"
#include <base/Logging.hpp>
#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis {
namespace io {

YamlWriter::YamlWriter()
{}

YamlWriter::~YamlWriter()
{}

void YamlWriter::write(const std::string& filename, const BaseGraph::Ptr& graph) const
{
    const char *fname = filename.c_str();
    std::ofstream fout(fname);
    if(!fout.is_open() || fout.fail())
    {
        std::string error_msg = std::string("graph_analysis::io::YamlWriter: failed to open file ") + filename + " for graph export";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    LOG_INFO("graph_analysis::io::YamlWriter: rendering graph to file \"%s\" ", fname);

    VertexIterator::Ptr nodeIt = graph->getVertexIterator();
    fout << "nodes:\n";
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();
        exportVertex(fout, vertex);
    }

    EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
    fout << "edges:" << std::endl;
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

void YamlWriter::exportVertex(std::ofstream& fout, Vertex::Ptr vertex) const
{
    fout << "  - id: " << vertex->getUid() << std::endl;
    fout << "    label: " << vertex->getLabel() << std::endl;
    fout << "    type: " << vertex->getClassName() << std::endl;
}

void YamlWriter::exportEdge(std::ofstream& fout, Edge::Ptr edge) const
{
    fout << "  - fromNodeId: " << edge->getSourceVertex()->getUid() << std::endl;
    fout << "    toNodeId: " << edge->getTargetVertex()->getUid() << std::endl;
    fout << "    label: " << edge->getLabel() << std::endl;
}

} // end namespace io
} // end namespace graph_analysis
