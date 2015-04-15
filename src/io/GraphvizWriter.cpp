#include "GraphvizWriter.hpp"
#include <base/Logging.hpp>

namespace graph_analysis {
namespace io {

GraphvizWriter::GraphvizWriter(const std::string &layout) : mLayout(layout)
{
    mpGVGraph = new graph_analysis::gui::GVGraph("GraphvizWriter");
}
GraphvizWriter::~GraphvizWriter()
{
    if(mpGVGraph)delete mpGVGraph;
}
void GraphvizWriter::write(const std::string& filename, const BaseGraph& graph) const
{
    VertexIterator::Ptr nodeIt = graph.getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();
        // Registering new node items
        graph_analysis::gui::NodeItem* nodeItem = graph_analysis::gui::NodeTypeManager::getInstance()->createItem(NULL, vertex);
        mpGVGraph->addNode(QString(nodeItem->getId().c_str()));
    }

    EdgeIterator::Ptr edgeIt = graph.getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();


    }

    mpGVGraph->renderToFile(filename, mLayout);
}

} // end namespace io
} // end namespace graph_analysis
