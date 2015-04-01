#ifndef GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_UNIFORM_RANDOM_REMOVAL
#define GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_UNIFORM_RANDOM_REMOVAL

#include <graph_analysis/percolation/Strategy.hpp>

namespace graph_analysis {
namespace percolation {
namespace strategies {

class UniformRandomRemoval : public Strategy
{
public:

    void apply(SubGraph::Ptr subgraph);
};

} // end namespace strategies
} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_UNIFORM_RANDOM_REMOVAL
