#include "GexfWriter.hpp"

#include <stdlib.h>
#include <libgexf/gexf.h>
#include <libgexf/filewriter.h>
#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <base/Logging.hpp>

namespace graph_analysis {
namespace io {

void GexfWriter::write(const std::string& filename, const BaseGraph::Ptr& graph) const
{
    libgexf::GEXF gexf;
    libgexf::DirectedGraph& digraph = gexf.getDirectedGraph();
    libgexf::Data& data = gexf.getData();

    std::string classAttr = "0";
    data.addNodeAttributeColumn(classAttr, "class", "STRING");
    data.addEdgeAttributeColumn(classAttr, "class", "STRING");

    VertexIterator::Ptr vit = graph->getVertexIterator();
    while(vit->next())
    {
        Vertex::Ptr vertex = vit->current();
        std::string nodeIdString = boost::lexical_cast<std::string>( graph->getVertexId(vertex) );
        digraph.addNode(nodeIdString);

        data.setNodeLabel(nodeIdString, vertex->toString() );
        data.setNodeValue(nodeIdString, classAttr, vertex->toString());
    }

    EdgeIterator::Ptr eit = graph->getEdgeIterator();
    while(eit->next())
    {
        Edge::Ptr edge = eit->current();
        std::string edgeId = boost::lexical_cast<std::string>( graph->getEdgeId(edge) );
        std::string srcNode = boost::lexical_cast<std::string>( graph->getVertexId( edge->getSourceVertex() ) );
        std::string targetNode = boost::lexical_cast<std::string>( graph->getVertexId( edge->getTargetVertex() ) );

        digraph.addEdge(edgeId, srcNode, targetNode);
        data.setEdgeLabel(edgeId, edge->toString() );
        data.setEdgeValue(edgeId, classAttr, edge->toString());
    }

    libgexf::FileWriter writer;
    boost::xpressive::sregex regex = boost::xpressive::as_xpr(".gexf");
    std::string replace = "";
    std::string name = boost::xpressive::regex_replace(filename, regex, replace);

    name = name + ".gexf";
    writer.init(name, &gexf);
    writer.write();

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
