#include "PortVertex.hpp"

namespace graph_analysis {

const int PortVertex::INVALID_PORT_ID = -1;

PortVertex::PortVertex(const std::string& label)
    : Vertex(label)
{}

} // end namespace graph_analysis
