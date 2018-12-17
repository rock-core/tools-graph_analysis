#include "Visitor.hpp"

namespace graph_analysis  {
namespace algorithms {

Visitor::Status Visitor::getStatus(const Vertex::Ptr& edge) const
{
    std::unordered_map<Vertex::Ptr, Status>::const_iterator cit = mVertexStatus.find(edge);
    if(cit != mVertexStatus.end())
    {
        return cit->second;
    }
    return UNKNOWN;
}

Visitor::Status Visitor::getStatus(const Edge::Ptr& edge) const
{
    std::unordered_map<Edge::Ptr, Status>::const_iterator cit = mEdgeStatus.find(edge);
    if(cit != mEdgeStatus.end())
    {
        return cit->second;
    }
    return UNKNOWN;
}

} // end algorithms
} // end graph_analysis
