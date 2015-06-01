#include "ClusterVertex.hpp"

namespace graph_analysis {


ClusterVertex::ClusterVertex(const std::string& label)
    : Vertex(label)
{}

Vertex::Ptr ClusterVertex::clone() const
{
    Vertex::Ptr vertex( getClone());
    return vertex;
}

} // end namespace graph_analysis
