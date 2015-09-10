#include "Port.hpp"

namespace graph_analysis {
namespace gui {

const int Port::INVALID_PORT_ID = -1;

Port::Port(const std::string& label)
    : Vertex(label)
{}

} // namespace gui
} // end namespace graph_analysis
