#include "OrogenModelReader.hpp"

#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <graph_analysis/gui/ComponentGraphEditor/Component.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/PortConnection.hpp>

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
    Vertex::Ptr vertex1 = vManager->createVertex("graph_analysis::Component", "node1");
    Vertex::Ptr vertex2 = vManager->createVertex("graph_analysis::Component", "node2");
    Vertex::Ptr vertex3 = vManager->createVertex("graph_analysis::InputPort", "iPort1");
    Vertex::Ptr vertex4 = vManager->createVertex("graph_analysis::OutputPort", "oPort");
    Vertex::Ptr vertex5 = vManager->createVertex("graph_analysis::OutputPort", "oPort2");
    Vertex::Ptr vertex6 = vManager->createVertex("graph_analysis::Component", "node3");
    Vertex::Ptr vertex7 = vManager->createVertex("graph_analysis::OutputPort", "oPort2");
    Vertex::Ptr vertex8 = vManager->createVertex("graph_analysis::InputPort", "iPort2");
    graph->addVertex(vertex1);
    graph->addVertex(vertex2);
    graph->addVertex(vertex3);
    graph->addVertex(vertex4);
    graph->addVertex(vertex5);
    graph->addVertex(vertex6);
    graph->addVertex(vertex7);
    graph->addVertex(vertex8);

    // fancier
    Component::Ptr comp = Component::Ptr(new Component("testcomp"));
    graph->addVertex(comp);
    OutputPort::Ptr port1 = OutputPort::Ptr(new OutputPort("bla"));
    graph->addVertex(port1);
    graph->addEdge(
        eManager->createEdge("graph_analysis::HasFeature", comp, port1, "has"));
    InputPort::Ptr port2 = InputPort::Ptr(new InputPort("blupp"));
    graph->addVertex(port2);
    graph->addEdge(
        eManager->createEdge("graph_analysis::HasFeature", comp, port2, "has"));
    PortConnection::Ptr conn = PortConnection::Ptr(new PortConnection(
        port1, dynamic_pointer_cast<InputPort>(vertex8), "testcomp"));
    graph->addEdge(conn);

    // now add the ports to the respective clusters via "hasFeature" label
    Edge::Ptr edge1 = eManager->createEdge("graph_analysis::HasFeature",
                                           vertex1, vertex3, "has");
    Edge::Ptr edge2 = eManager->createEdge("graph_analysis::HasFeature",
                                           vertex2, vertex5, "has");
    Edge::Ptr edge3 = eManager->createEdge("graph_analysis::HasFeature",
                                           vertex2, vertex4, "has");
    Edge::Ptr edge4 = eManager->createEdge("graph_analysis::PortConnection",
                                           vertex4, vertex3, "some-connection");

    Edge::Ptr edge5 = eManager->createEdge("graph_analysis::HasFeature",
                                           vertex1, vertex7, "has");
    Edge::Ptr edge6 = eManager->createEdge("graph_analysis::HasFeature",
                                           vertex1, vertex8, "has");
    graph->addEdge(edge1);
    graph->addEdge(edge2);
    graph->addEdge(edge3);
    graph->addEdge(edge4);
    graph->addEdge(edge5);
    graph->addEdge(edge6);
}

} // end namespace io
} // end namespace graph_analysis
