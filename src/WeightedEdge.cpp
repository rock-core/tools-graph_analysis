#include "WeightedEdge.hpp"

namespace graph_analysis {

WeightedEdge::WeightedEdge(double weight)
    : Edge()
    , mWeight(weight)
{}

WeightedEdge::WeightedEdge(Vertex::Ptr source, Vertex::Ptr target, double weight)
    : Edge(source, target)
    , mWeight(weight)
{}

/**
 * Convert element to string
 */
std::string WeightedEdge::toString() const
{
    std::stringstream ss;
    ss << Edge::toString();
    ss << ": " << mWeight;
    return ss.str();
}

} // end namespace graph_analysis
