#include "GraphIO.hpp"
#include <sstream>
#include <boost/assign/list_of.hpp>

#include "io/GexfWriter.hpp"
#include "io/GexfReader.hpp"

#include "io/GraphvizWriter.hpp"

namespace graph_analysis {
namespace representation {

std::map<Type, std::string> TypeTxt = boost::assign::map_list_of
    (UNKNOWN, "UNKNOWN")
    (GEXF, "GEXF")
    (LEMON, "LEMON")
    (GRAPHVIZ, "GRAPHVIZ")
    ;

} // end namespace representation

namespace io {


GraphIO::WriterMap GraphIO::msWriters = boost::assign::map_list_of
    (representation::GEXF, Writer::Ptr( new GexfWriter()))
    (representation::GRAPHVIZ, Writer::Ptr( new GraphvizWriter()))
    ;

GraphIO::ReaderMap GraphIO::msReaders = boost::assign::map_list_of
    (representation::GEXF, Reader::Ptr( new GexfReader()))
    ;

void GraphIO::write(const std::string& filename, const BaseGraph& graph, representation::Type format)
{
    WriterMap::const_iterator cit = msWriters.find(format);
    if(cit != msWriters.end())
    {
        Writer::Ptr writer = cit->second;
        writer->write(filename, graph);
    } else {
        std::stringstream ss;
        ss << "GraphIO: writing format ";
        ss << "' " << representation::TypeTxt[format] << "'";
        ss << " is not supported";

        throw std::runtime_error(ss.str());
    }
}

void GraphIO::read(const std::string& filename, BaseGraph& graph, representation::Type format)
{
    ReaderMap::const_iterator cit = msReaders.find(format);
    if(cit != msReaders.end())
    {
        Reader::Ptr reader = cit->second;
        reader->read(filename, graph);
    } else {
        std::stringstream ss;
        ss << "GraphIO: reading format ";
        ss << "' " << representation::TypeTxt[format] << "'";
        ss << " is not supported";

        throw std::runtime_error(ss.str());
    }
}

} // end namespace io
} // end namespace graph_analysis
