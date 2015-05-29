#include "BaseVertex.hpp"

namespace graph_analysis {


BaseVertex::BaseVertex(const std::string& label)
    : Vertex(label)
{}

Vertex::Ptr BaseVertex::clone() const
{
    Vertex::Ptr vertex( getClone());
    vertex->disassociateFromAll();
    return vertex;
}

} // end namespace graph_analysis
