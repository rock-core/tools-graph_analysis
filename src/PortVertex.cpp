#include "PortVertex.hpp"

namespace graph_analysis {


PortVertex::PortVertex(const std::string& label)
    : Vertex(label)
{}

Vertex::Ptr PortVertex::clone() const
{
    Vertex::Ptr vertex( getClone());
    return vertex;
}

} // end namespace graph_analysis
