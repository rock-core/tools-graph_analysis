#include "GexfWriter.hpp"
#include <stdlib.h>
#include <libgexf/gexf.h>
#include <libgexf/filewriter.h>
#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <base-logging/Logging.hpp>
#include "../VertexTypeManager.hpp"
#include "../EdgeTypeManager.hpp"

namespace graph_analysis {
namespace io {

void GexfWriter::write(const std::string& filename, const BaseGraph& graph) const
{
    BaseGraph graph_copy = graph;
    write(filename, BaseGraph::Ptr(&graph_copy)); // reusing code
}

void GexfWriter::write(const std::string& filename, const BaseGraph::Ptr& graph) const
{
    // loading gexf lib main components
    libgexf::GEXF gexf;
    libgexf::DirectedGraph& digraph = gexf.getDirectedGraph();
    libgexf::Data& data = gexf.getData();

    std::string classAttr = CLASS; // see "GraphIO.hpp"
    data.addNodeAttributeColumn(classAttr, "class", "STRING");
    //data.setNodeAttributeDefault(classAttr,"")
    data.addEdgeAttributeColumn(classAttr, "class", "STRING");
    //data.setEdgeAttributeDefault(classAttr,"")

    std::string labelAttr = LABEL; // see "GraphIO.hpp"
    data.addNodeAttributeColumn(labelAttr, "label", "STRING");
    data.addEdgeAttributeColumn(labelAttr, "label", "STRING");

    // Add custom attribute serialization for vertex types that have been
    // registered in the vertex type manager
    VertexTypeManager *vManager = VertexTypeManager::getInstance();
    for(const Attribute& attribute : vManager->getKnownAttributes())
    {
        LOG_DEBUG_S << "Adding custom node attribute: " << attribute.toString();
        data.addNodeAttributeColumn(attribute.getId(), attribute.getName(), "STRING");
    }

    // loading the nodes and their attributes to the gexf components
    VertexIterator::Ptr vit = graph->getVertexIterator();
    while(vit->next())
    {
        Vertex::Ptr vertex = vit->current();
        std::string nodeIdString = boost::lexical_cast<std::string>( graph->getVertexId(vertex) );
        digraph.addNode(nodeIdString);

        data.setNodeLabel(nodeIdString, vertex->getLabel() );
        data.setNodeValue(nodeIdString, classAttr, vertex->getClassName());
        data.setNodeValue(nodeIdString, labelAttr, vertex->getLabel());

        std::vector< std::pair<Attribute::Id, std::string> > attributeAssignments= vManager->getAttributeValues(vertex);
        for(const std::pair<Attribute::Id, std::string>& assignment : attributeAssignments)
        {
            data.setNodeValue(nodeIdString, assignment.first, assignment.second);
        }
    }

    // Add custom atttribute serialization for vertex types that have been
    // registered in the vertex type manager
    EdgeTypeManager *eManager = EdgeTypeManager::getInstance();
    for(const Attribute& attribute : eManager->getKnownAttributes())
    {
        data.addEdgeAttributeColumn(attribute.getId(), attribute.getName(), "STRING");
    }

    // loading the edges and their attributes to the gexf components
    EdgeIterator::Ptr eit = graph->getEdgeIterator();
    while(eit->next())
    {
        Edge::Ptr edge = eit->current();
        std::string edgeId = boost::lexical_cast<std::string>( graph->getEdgeId(edge) );
        std::string srcNode = boost::lexical_cast<std::string>( graph->getVertexId( edge->getSourceVertex() ) );
        std::string targetNode = boost::lexical_cast<std::string>( graph->getVertexId( edge->getTargetVertex() ) );

        digraph.addEdge(edgeId, srcNode, targetNode);
        data.setEdgeLabel(edgeId, edge->getLabel() );
        data.setEdgeValue(edgeId, classAttr, edge->getClassName());
        data.setEdgeValue(edgeId, labelAttr, edge->getLabel());


        std::vector< std::pair<Attribute::Id, std::string> > attributes = eManager->getAttributeValues(edge);
        for(const std::pair<Attribute::Id, std::string>& attribute : attributes)
        {
            data.setEdgeValue(edgeId, attribute.first, attribute.second);
        }
    }

    // calling the gexf lib renderer
    libgexf::FileWriter writer;
    boost::xpressive::sregex regex = boost::xpressive::as_xpr(".gexf");
    std::string replace("");
    std::string name = boost::xpressive::regex_replace(filename, regex, replace);

    name = name + ".gexf";
    writer.init(name, &gexf);
    writer.write();

    if(std::string::npos != name.find(' '))
    {
        // disregarding the formatting stage for filenames containing blanks
        return;
    }

    // the formatting stage
    std::string formattedFile = name + ".formatted";
    std::string command = "`which xmllint` --encode UTF-8 --format " + name + " > " + formattedFile;

    LOG_INFO("Trying to format gexf using xmllint: '%s'", command.c_str());
    if( system(command.c_str()) == 0 )
    {
        command = "mv " + formattedFile + " " + name;
        if( system(command.c_str()) )
        {
            throw std::runtime_error("graph_analysis::io::GexfWriter::write: Failed to rename file after performing xmlling");
        }
    } else {
        LOG_INFO("Gexf file '%s' written, but proper formatting failed -- make sure that xmllint is installed", name.c_str());
    }
}

} // end namespace io
} // end namespace graph_analysis
