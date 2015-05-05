#include "GraphvizWriter.hpp"
#include <base/Logging.hpp>

namespace graph_analysis {
namespace io {

GraphvizWriter::GraphvizWriter(const std::string &layout) : mLayout(layout)
{}

GraphvizWriter::~GraphvizWriter()
{}

void GraphvizWriter::write(const std::string& filename, const BaseGraph& graph) const
{
    graph_analysis::gui::GVGraph gvGraph("GraphvizGraph");

    typedef std::map<graph_analysis::Vertex::Ptr, graph_analysis::gui::NodeItem*> NodeItemMap;
    // Allow mapping from graph vertexes to nodes in the scene
    NodeItemMap nodeItemMap;

    VertexIterator::Ptr nodeIt = graph.getVertexIterator();
    LOG_INFO("GraphvizWriter: Formatting Graphviz nodes");
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();
        // Registering new node items
        graph_analysis::gui::NodeItem* nodeItem = new graph_analysis::gui::NodeItem();
        nodeItem->setVertex(vertex);
        std::string uniqueLabel = vertex->toPrefixedString(graph.getId());
        gvGraph.addNode(QString( uniqueLabel.c_str() ) );
        nodeItemMap[vertex] = nodeItem;
    }
    LOG_INFO("GraphvizWriter: Done formatting Graphviz nodes");

    LOG_INFO("GraphvizWriter: Formatting Graphviz edges");
    EdgeIterator::Ptr edgeIt = graph.getEdgeIterator();

    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        // Registering new node edge items
        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();
        std::string uniqueSourceLabel = source->toPrefixedString(graph.getId());
        std::string uniqueTargetLabel = target->toPrefixedString(graph.getId());

        graph_analysis::gui::NodeItem* sourceNodeItem = nodeItemMap[ source ];
        graph_analysis::gui::NodeItem* targetNodeItem = nodeItemMap[ target ];

        if(!sourceNodeItem || !targetNodeItem)
        {
            continue;
        }
        gvGraph.addEdge(QString( uniqueSourceLabel.c_str()), QString( uniqueTargetLabel.c_str()), graph.getEdgeId(edge), QString( edge->getLabel().c_str()));
    }
    LOG_INFO("GraphvizWriter: Done formatting Graphviz edges");

    LOG_INFO("GraphvizWriter: Applying default layout such that GVGraph context is not empty");
    gvGraph.applyLayout();
    LOG_INFO("GraphvizWriter: rendering GVGraph to file \"%s\" by layout \"%s\"", filename.c_str(), mLayout.c_str());
    gvGraph.renderToFile(filename, mLayout);
    LOG_INFO("GraphvizWriter: done rendering GVGraph to file \"%s\"", filename.c_str());
}

} // end namespace io
} // end namespace graph_analysis
