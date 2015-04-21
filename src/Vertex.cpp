#include "Vertex.hpp"

namespace graph_analysis {

Vertex::Vertex(const std::string& label)
    : mLabel(label)
{}

std::string Vertex::toString() const
{
    if(!mLabel.empty())
    {
        return mLabel;
    } else {
        return getClassName();
    }
}

} // end namespace graph_analysis
