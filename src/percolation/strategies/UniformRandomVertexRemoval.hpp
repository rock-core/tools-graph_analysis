#ifndef GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_UNIFORM_RANDOM_VERTEX_REMOVAL
#define GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_UNIFORM_RANDOM_VERTEX_REMOVAL

#include <graph_analysis/percolation/Strategy.hpp>

namespace graph_analysis {
namespace percolation {
    class RandomNumberGenerator;

namespace strategies {

class UniformRandomVertexRemoval : public Strategy
{
    std::vector<Vertex::Ptr> mDisabled;
    double mOccupationProbability;
    RandomNumberGenerator* mpRandomNumberGenerator;

public:
    UniformRandomVertexRemoval(double occupationProbability, uint32_t seed = 0);

    EventList apply(SubGraph::Ptr subgraph);

    std::vector<Vertex::Ptr> getDisabledVertices() const { return mDisabled; }
};

} // end namespace strategies
} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_UNIFORM_RANDOM_VERTEX_REMOVAL
