#ifndef GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_RANDOM_REMOVAL_HPP
#define GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_RANDOM_REMOVAL_HPP

namespace graph_analysis {
namespace percolation {
namespace strategies {

class RandomRemoval
{
public:
    RandomRemoval(SubGraph::Ptr subGraph);

    void setProbabilityDensity(GraphElementId id);

    float getProbability(GraphElementId id) const;

private:
    // Store probability
    std::map<GraphElementId, uint32_t> mProbabilityDensity;

};

} // end namespace strategies
} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_RANDOM_REMOVAL_HPP
