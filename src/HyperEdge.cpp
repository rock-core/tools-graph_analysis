#include "HyperEdge.hpp"
#include "BaseGraph.hpp"
#include <algorithm>

namespace graph_analysis {


HyperEdge::HyperEdge(const std::string& label)
    : Vertex(label)
{}

HyperEdge::HyperEdge(const Vertex::PtrList& vertices, const std::string& label)
    : Vertex(label)
    , mVertices(vertices)
{
}

std::string HyperEdge::getClassName() const
{
    return "graph_analysis::HyperEdge";
}

std::string HyperEdge::toString() const
{
    return toString(0);
}

std::string HyperEdge::toString(uint32_t indent) const
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

Vertex::PtrList HyperEdge::getInvolvedVertices(const HyperEdge::Ptr& e0, const HyperEdge::Ptr& e1)
{
    Vertex::PtrSet vertices;
    vertices.insert(e0->getVertices().begin(), e0->getVertices().end());
    vertices.insert(e1->getVertices().begin(), e1->getVertices().end());
    return Vertex::PtrList(vertices.begin(), vertices.end());
}

bool HyperEdge::areMeeting(const HyperEdge::Ptr& e0, const HyperEdge::Ptr& e1)
{
    Vertex::PtrList::const_iterator v0It = e0->getVertices().begin();
    for(; v0It != e0->getVertices().end(); ++v0It)
    {
        Vertex::PtrList::const_iterator v1It = e1->getVertices().begin();
        for(; v1It != e1->getVertices().end(); ++v1It)
        {
            if(*v0It == *v1It)
            {
                return true;
            }
        }
    }
    return false;
}

} // end namespace graph_analysis
