#include "WeightedVertex.hpp"

namespace graph_analysis {

WeightedVertex::WeightedVertex(double weight)
    : Vertex()
    , mWeight(weight)
{}

/**
 * Convert element to string
 */
std::string WeightedVertex::toString() const
{
    std::stringstream ss;
    ss << Vertex::toString();
    ss << ": " << mWeight;
    return ss.str();
}

} // end namespace graph_analysis
