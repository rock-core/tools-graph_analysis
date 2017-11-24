#include "DirectedHyperEdge.hpp"
#include <algorithm>

namespace graph_analysis {


DirectedHyperEdge::DirectedHyperEdge(const std::string& label)
    : HyperEdge(label)
{}

DirectedHyperEdge::DirectedHyperEdge(const Vertex::PtrList& fromVertices,
        const Vertex::PtrList& toVertices,
        const std::string& label)
    : HyperEdge(label)
    , mSourceVertices(fromVertices)
    , mTargetVertices(toVertices)
{
    mVertices.insert(mVertices.begin(), mSourceVertices.begin(), mSourceVertices.end());
    mVertices.insert(mVertices.begin(), mTargetVertices.begin(), mTargetVertices.end());
}

std::string DirectedHyperEdge::getClassName() const
{
    return "graph_analysis::DirectedHyperEdge";
}

std::string DirectedHyperEdge::toString() const
{
    return toString(0);
}

std::string DirectedHyperEdge::toString(uint32_t indent) const
{
    std::string hspace(indent,' ');
    if(!mLabel.empty())
    {
        return hspace + mLabel;
    }
    else
    {
        return hspace + getClassName();
    }
}
} // end namespace graph_analysis
