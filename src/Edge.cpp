#include "Edge.hpp"

namespace graph_analysis {

Edge::Edge(const std::string& label)
    : GraphElement()
    , mLabel(label)
{}

Edge::Edge(Vertex::Ptr source, Vertex::Ptr target, const std::string& label)
    : mSourceVertex(source)
    , mTargetVertex(target)
    , mLabel(label)
{}

std::string Edge::getClassName() const
{
    return "graph_analysis::Edge";
}

std::string Edge::toString() const
{
    std::stringstream ss;
    ss << (mLabel.empty() ? getClassName() : mLabel) << ":";
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
