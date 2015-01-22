#include "Vertex.hpp"

namespace graph_analysis {

Vertex::Vertex()
{}

Vertex::Vertex(const std::string& name)
    : mName(name)
{}

std::string Vertex::toString() const
{
    if(!mName.empty())
    {
        return mName;
    } else {
        return getClassName();
    }
}

} // end namespace graph_analysis
