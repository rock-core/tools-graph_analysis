#include "Vertex.hpp"
#include <boost/assign/list_of.hpp>

namespace graph_analysis {


std::set<std::string> Vertex::msSupportedTypes = boost::assign::list_of
    ("base")
    ("port")
    ("cluster")
    ;

Vertex::Vertex(const std::string& label, const std::string& type)
    : GraphElement(label, type)
{}

Vertex::Ptr Vertex::clone() const
{
    Vertex::Ptr vertex( getClone());
    vertex->disassociateFromAll();
    return vertex;
}

std::string Vertex::toString() const
{
    if(!mLabel.empty())
    {
        return mLabel;
    }
    else
    {
        return getClassName();
    }
}

} // end namespace graph_analysis
