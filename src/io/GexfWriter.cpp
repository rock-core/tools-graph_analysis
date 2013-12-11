#include "GexfWriter.hpp"

#include <stdlib.h>
#include <gexf/libgexf.h>
#include <boost/lexical_cast.hpp>

namespace graph_analysis {
namespace io {

void GexfWriter::write(const std::string& filename, const BaseGraph& graph) const
{
    libgexf::GEXF gexf;
    libgexf::DirectedGraph& digraph = gexf.getDirectedGraph();
    libgexf::Data& data = gexf.getData();



    std::string classAttr = "0";
    data.addNodeAttributeColumn(classAttr, "class", "STRING");
    data.addEdgeAttributeColumn(classAttr, "class", "STRING");

    VertexIterator::Ptr vit = const_cast<BaseGraph*>(&graph)->getVertexIterator();
    while(vit->next())
    {
        Vertex::Ptr vertex = vit->current();
        std::string nodeIdString = boost::lexical_cast<std::string>( graph.getVertexId(vertex) );
        digraph.addNode(nodeIdString);

        data.setNodeLabel(nodeIdString, vertex->toString() );
        data.setNodeValue(nodeIdString, classAttr, vertex->toString());
    }

    EdgeIterator::Ptr eit = const_cast<BaseGraph*>(&graph)->getEdgeIterator();
    while(eit->next())
    {
        Edge::Ptr edge = eit->current();
        std::string edgeId = boost::lexical_cast<std::string>( graph.getEdgeId(edge) );
        std::string srcNode = boost::lexical_cast<std::string>( graph.getVertexId( edge->getSourceVertex() ) );
        std::string targetNode = boost::lexical_cast<std::string>( graph.getVertexId( edge->getTargetVertex() ) );

        digraph.addEdge(edgeId, srcNode, targetNode);
        data.setEdgeLabel(edgeId, edge->toString() );
        data.setEdgeValue(edgeId, classAttr, edge->toString());
    }

    libgexf::FileWriter writer;
    std::string name = filename + ".gexf";
    writer.init(name, &gexf);
    writer.write();
}

} // end namespace io
} // end namespace graph_analysis
