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

std::vector<Vertex::Ptr> Edge::getInvolvedVertices(Edge::Ptr e0, Edge::Ptr e1)
{
    std::vector<Vertex::Ptr> vertices;
    Vertex::Ptr e0_source = e0->getSourceVertex();
    Vertex::Ptr e0_target = e0->getTargetVertex();
    if(e0_source == e1->getSourceVertex() || e0_source == e1->getTargetVertex())
    {
        // skip e0_source
    } else {
        vertices.push_back(e0_source);
    }

    if(e0_target == e1->getSourceVertex() || e0_target == e1->getTargetVertex())
    {
        // skip e0_target
    } else {
        vertices.push_back(e0_source);
    }

    vertices.push_back(e1->getSourceVertex());
    vertices.push_back(e1->getTargetVertex());

    return vertices;
}

bool Edge::areMeeting(Edge::Ptr e0, Edge::Ptr e1)
{
    return getInvolvedVertices(e0, e1).size() < 4;
}

}
