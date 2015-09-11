#include "GexfReader.hpp"
#include <base/Logging.hpp>
#include <libgexf/nodeiter.h>
#include <libgexf/edgeiter.h>
#include <libgexf/filereader.h>
#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

namespace graph_analysis {
namespace io {

void GexfReader::read(const std::string& filename, BaseGraph::Ptr graph)
{
    // loading relevant gexf lib components
    libgexf::FileReader* reader = new libgexf::FileReader();
    reader->init(filename.c_str());
    reader->slurp(); // TODO: gexf.xsd schema needs to be downloaded and locally stored for a viable validation test with libgexf::SchemaValidator (see http://gexf.net/format/schema.html)

    // unwrapping the already parsed graph
    libgexf::GEXF gexf = reader->getGEXFCopy();
    libgexf::DirectedGraph& gexf_graph = gexf.getDirectedGraph();
    libgexf::Data& data = gexf.getData();
    std::string classAttr = CLASS;
    std::string labelAttr = LABEL;

    // storing the graph elements to the internal baseGraph
    graph->clear();
    VertexMap vertexMap;
    // storing the nodes and their properties
    libgexf::NodeIter* node_it = gexf_graph.getNodes();
    while(node_it->hasNext())
    {
        libgexf::t_id current = node_it->next();
        std::string nodeClass = data.getNodeAttribute(current, classAttr);
        std::string nodeLabel = data.getNodeAttribute(current, labelAttr);
        Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(nodeClass, nodeLabel); // (new Vertex(nodeLabel));
        graph->addVertex(vertex);
        vertexMap[current] = vertex;
    }
    // storing the edges and their properties
    libgexf::EdgeIter* edge_it = gexf_graph.getEdges();
    while(edge_it->hasNext())
    {
        libgexf::t_id current = edge_it->next();
        std::string edgeClass = data.getEdgeAttribute(current, classAttr);
        std::string edgeLabel = data.getEdgeAttribute(current, labelAttr);
        Vertex::Ptr sourceVertex = vertexMap[edge_it->currentSource()]; // NOTE: assumes the .gexf(.xml) file is valid
        Vertex::Ptr targetVertex = vertexMap[edge_it->currentTarget()]; // NOTE: assumes the .gexf(.xml) file is valid
        Edge::Ptr edge = EdgeTypeManager::getInstance()->createEdge(edgeClass, sourceVertex, targetVertex, edgeLabel);
        graph->addEdge(edge);
    }
}

} // end namespace io
} // end namespace graph_analysis
