#ifndef GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_UNIFORM_RANDOM_EDGE_REMOVAL
#define GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_UNIFORM_RANDOM_EDGE_REMOVAL

#include <graph_analysis/percolation/Strategy.hpp>

namespace graph_analysis {
namespace percolation {
    class RandomNumberGenerator;

namespace strategies {

class UniformRandomEdgeRemoval : public Strategy
{
    std::vector<Edge::Ptr> mDisabled;
    double mOccupationProbability;
    RandomNumberGenerator* mpRandomNumberGenerator;

public:
    UniformRandomEdgeRemoval(double occupationProbability, uint32_t seed = 0);

    virtual ~UniformRandomEdgeRemoval() {}

    EventList apply(SubGraph::Ptr subgraph);

    std::vector<Edge::Ptr> getDisabledVertices() const { return mDisabled; }
};

} // end namespace strategies
} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_UNIFORM_RANDOM_EDGE_REMOVAL
