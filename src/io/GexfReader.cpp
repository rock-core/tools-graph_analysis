#include "GexfReader.hpp"

#include <libgexf/nodeiter.h>
#include <libgexf/edgeiter.h>
#include <libgexf/filereader.h>
#include <base/Logging.hpp>

namespace graph_analysis {
namespace io {

void GexfReader::read(const std::string& filename, const BaseGraph::Ptr& graph) const
{
    libgexf::FileReader* reader = new libgexf::FileReader();
    reader->init(filename.c_str());
    reader->slurp(); // TODO: gexf.xsd schema needs to be downloaded and locally stored for a viable validation test with libgexf::SchemaValidator (see http://gexf.net/format/schema.html)

    libgexf::GEXF gexf = reader->getGEXFCopy();
    libgexf::DirectedGraph& gexf_graph = gexf.getDirectedGraph();

    graph->clear();
    VertexMap vertexMap;
    libgexf::NodeIter* node_it = gexf_graph.getNodes();
    while(node_it->hasNext())
    {
        libgexf::t_id current = node_it->next();
        Vertex::Ptr vertex(new Vertex(current));
        graph->addVertex(vertex);
        vertexMap[current] = vertex;
    }
    libgexf::EdgeIter* edge_it = gexf_graph.getEdges();
    while(edge_it->hasNext())
    {
        libgexf::t_id current = edge_it->next();
        Vertex::Ptr sourceVertex = vertexMap[edge_it->currentSource()]; // NOTE: assumes .gexf(.xml) file is valid
        Vertex::Ptr targetVertex = vertexMap[edge_it->currentTarget()]; // NOTE: assumes .gexf(.xml) file is valid
        graph->addEdge(Edge::Ptr(new Edge(sourceVertex, targetVertex, current)));
    }
}

} // end namespace io
} // end namespace graph_analysis
