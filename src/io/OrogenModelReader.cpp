#include "OrogenModelReader.hpp"

#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

namespace graph_analysis {
namespace io {

void OrogenModelReader::read(const std::string &filename,
                             BaseGraph::Ptr graph) {
    // remove previous graph
    graph->clear();

    // handy pointers
    VertexTypeManager *vManager = VertexTypeManager::getInstance();
    EdgeTypeManager *eManager = EdgeTypeManager::getInstance();

    // how this works: create a "Cluster" as top-level vertex. then create each
    // individual input and output port. add them all to the graph:
    Vertex::Ptr vertex1 = vManager->createVertex("graph_analysis::Cluster", "node1");
    Vertex::Ptr vertex2 = vManager->createVertex("graph_analysis::Cluster", "node2");
    Vertex::Ptr vertex3 = vManager->createVertex("graph_analysis::InputPort", "iPort");
    Vertex::Ptr vertex4 = vManager->createVertex("graph_analysis::OutputPort", "oPort");
    Vertex::Ptr vertex5 = vManager->createVertex("graph_analysis::OutputPort", "oPort2");
    Vertex::Ptr vertex6 = vManager->createVertex("graph_analysis::Cluster", "node3");
    graph->addVertex(vertex1);
    graph->addVertex(vertex2);
    graph->addVertex(vertex3);
    graph->addVertex(vertex4);
    graph->addVertex(vertex5);
    graph->addVertex(vertex6);

    // now add the ports to the respective clusters via "hasFeature" label
    Edge::Ptr edge1 = eManager->createEdge("bla", vertex1, vertex3, "hasFeature");
    Edge::Ptr edge2 = eManager->createEdge("bla", vertex2, vertex5, "hasFeature");
    Edge::Ptr edge3 = eManager->createEdge("graph_analysis::Edge", vertex2, vertex4, "hasFeature");
    Edge::Ptr edge4 = eManager->createEdge("PortConnection", vertex3, vertex4,
                                           "some-connection");
    graph->addEdge(edge1);
    graph->addEdge(edge2);
    graph->addEdge(edge3);
    graph->addEdge(edge4);
}

} // end namespace io
} // end namespace graph_analysis
