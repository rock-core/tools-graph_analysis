#include "Edge.hpp"

namespace graph_analysis {

Edge::Edge()
    : GraphElement()
{}

Edge::Edge(Vertex::Ptr source, Vertex::Ptr target)
    : mSourceVertex(source)
    , mTargetVertex(target)
{}

std::string Edge::getClassName() const
{
    return "graph_analysis::Edge";
}

std::string Edge::toString() const
{
    std::stringstream ss;
    ss << getClassName() << ":";
    if(mSourceVertex)
    {
        ss << mSourceVertex->toString();
    } else {
        ss << "n/a";
    }
    ss << "->";
    if(mTargetVertex)
    {
        ss << mTargetVertex->toString();
    } else {
        ss << "n/a";
    }
    return ss.str();
}


}
