#include "GraphMLWriter.hpp"
#include <fstream>
#include <sstream>
#include "../VertexTypeManager.hpp"
#include "../EdgeTypeManager.hpp"

namespace graph_analysis {
namespace io {

void GraphMLWriter::write(const std::string& filename, const BaseGraph& graph) const
{
    BaseGraph graph_copy = graph;
    write(filename, BaseGraph::Ptr(&graph_copy));
}

void GraphMLWriter::write(const std::string& filename, const BaseGraph::Ptr& graph) const
{
    std::ofstream outfile;
    outfile.open(filename);

    size_t graphId = graph->getId();
    VertexTypeManager* vManager = VertexTypeManager::getInstance();
    EdgeTypeManager* eManager = EdgeTypeManager::getInstance();

    outfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    outfile << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" ";
    outfile << " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << std::endl;
    outfile << " xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">" << std::endl;

    // TBD: for each graph
    outfile << "  <graph id=\"" << graphId << "\" edgedefault=";
    if(graph->isDirected())
    {
        outfile << "\"directed\"";
    } else {
        outfile << "\"undirected\"";
    }
    outfile << ">" << std::endl;

    std::set<std::string> nodeTypes;
    std::set<std::string> edgeTypes;
    std::string edgelist = generateEdgeList(graph,4,edgeTypes);
    std::string nodelist = generateNodeList(graph,4,nodeTypes);

    std::string keylist = generateKeyList(nodeTypes, edgeTypes, 4);

    outfile << keylist << std::endl;
    outfile << nodelist << std::endl;
    outfile << edgelist << std::endl;

    outfile << "  </graph>" << std::endl;
    outfile << "</graphml>" << std::endl;
    outfile.close();
}

std::string GraphMLWriter::generateKeyList(const std::set<std::string>& vertexTypes,
        const std::set<std::string>& edgeTypes,
        size_t indent) const
{
    std::string hspace(indent,' ');
    // Attribute 'class
    std::stringstream keys;
    keys << hspace << "<key id=\"0\" for=\"node\" attr.name=\"class\" attr.type=\"string\">" << std::endl;
    keys << hspace << "  <default>" << Vertex().getClassName() << "</default>" << std::endl;
    keys << hspace << "</key>" << std::endl;
    keys << hspace << "<key id=\"0\" for=\"edge\" attr.name=\"class\" attr.type=\"string\">" << std::endl;
    keys << hspace << "  <default>" << Edge().getClassName() << "</default>" << std::endl;
    keys << hspace << "</key>" << std::endl;

    // Attribute 'label'
    keys << hspace << "<key id=\"1\" for=\"node\" attr.name=\"label\" attr.type=\"string\">" << std::endl;
    keys << hspace << "  <default></default>" << std::endl;
    keys << hspace << "</key>" << std::endl;
    keys << hspace << "<key id=\"1\" for=\"edge\" attr.name=\"label\" attr.type=\"string\">" << std::endl;
    keys << hspace << "  <default></default>" << std::endl;
    keys << hspace << "</key>" << std::endl;

    // Attribute 'uuid'
    keys << hspace << "<key id=\"2\" for=\"node\" attr.name=\"uuid\" attr.type=\"string\">" << std::endl;
    keys << hspace << "  <default></default>" << std::endl;
    keys << hspace << "</key>" << std::endl;
    keys << hspace << "<key id=\"2\" for=\"edge\" attr.name=\"uuid\" attr.type=\"string\">" << std::endl;
    keys << hspace << "  <default></default>" << std::endl;
    keys << hspace << "</key>" << std::endl;

    VertexTypeManager* vManager = VertexTypeManager::getInstance();
    EdgeTypeManager* eManager = EdgeTypeManager::getInstance();

    // Custom attributes
    for(const Attribute& attribute : vManager->getKnownAttributes(vertexTypes))
    {
        keys << hspace << "<key id=\"" << attribute.getId() << "\" for=\"node\" attr.name=\"" << attribute.getName() << "\" attr.type=\"string\">" << std::endl;
        keys << hspace << "  <default></default>" << std::endl;
        keys << hspace << "</key>" << std::endl;
    }
    for(const Attribute& attribute : eManager->getKnownAttributes(edgeTypes))
    {
        keys << hspace << "<key id=\"" << attribute.getId() << "\" for=\"edge\" attr.name=\"" << attribute.getName() << "\" attr.type=\"string\">" << std::endl;
        keys << hspace << "  <default></default>" << std::endl;
        keys << hspace << "</key>" << std::endl;
    }
    return keys.str();
}

std::string GraphMLWriter::generateNodeList(const BaseGraph::Ptr& graph,
        size_t indent,
        std::set<std::string>& vertexTypes
        ) const
{
    std::stringstream ss;
    std::string hspace(indent, ' ');
    VertexIterator::Ptr vertexIt = graph->getVertexIterator();
    VertexTypeManager* vManager = VertexTypeManager::getInstance();
    while(vertexIt->next())
    {
        const Vertex::Ptr& v = vertexIt->current();
        vertexTypes.insert(v->getClassName());
        ss << hspace << "<node id=\"" << v->getId(graph->getId()) << "\">" << std::endl;
        ss << hspace << "  <data key=\"0\">" << v->getClassName() << "</data>" << std::endl;
        ss << hspace << "  <data key=\"1\">" << v->getLabel() << "</data>" << std::endl;
        ss << hspace << "  <data key=\"2\">" << v->getUuid() << "</data>" << std::endl;

        std::vector< std::pair<Attribute::Id, std::string> > attributeAssignments = vManager->getAttributeValues(v);

        for(const std::pair<Attribute::Id, std::string>& assignment : attributeAssignments)
        {
            ss << hspace << "  <data key=\"" << assignment.first << "\">"
                << assignment.second << "</data>" << std::endl;
        }
        ss << hspace << "</node>" << std::endl;
    }
    return ss.str();

}
std::string GraphMLWriter::generateEdgeList(const BaseGraph::Ptr& graph,
        size_t indent,
        std::set<std::string>& edgeTypes
        ) const
{
    std::stringstream ss;
    std::string hspace(indent, ' ');

    EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
    EdgeTypeManager* eManager = EdgeTypeManager::getInstance();

    size_t graphId = graph->getId();

    while(edgeIt->next())
    {
        const Edge::Ptr& e = edgeIt->current();
        edgeTypes.insert(e->getClassName());
        ss << hspace << "<edge id=\"" << e->getId(graphId) << "\" ";
        ss << hspace << "source=\"" << e->getSourceVertex()->getId( graphId ) << "\" ";
        ss << hspace << "target=\"" << e->getTargetVertex()->getId( graphId ) << "\">\n";
        ss << hspace << "  <data key=\"0\">" << e->getClassName() << "</data>" << std::endl;
        ss << hspace << "  <data key=\"1\">" << e->getLabel() << "</data>" << std::endl;
        ss << hspace << "  <data key=\"2\">" << e->getUuid() << "</data>" << std::endl;
        std::vector< std::pair<Attribute::Id, std::string> > attributeAssignments = eManager->getAttributeValues(e);
        for(const std::pair<Attribute::Id, std::string>& assignment : attributeAssignments)
        {
            ss << hspace << "  <data key=\"" << assignment.first << "\">"
                << assignment.second << "</data>" << std::endl;
        }
        ss << hspace << "</edge>" << std::endl;
    }
    return ss.str();
}

} // end namespace io
} // end namespace graph_analysis
