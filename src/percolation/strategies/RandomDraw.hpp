#ifndef GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_RANDOM_DRAW_HPP
#define GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_RANDOM_DRAW_HPP

#include <map>
#include <stdint.h>
#include <gsl/gsl_rng.h>
#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis {
namespace percolation {
namespace strategies {

/**
 * \details
 * You can use the environoment variable GSL_RNG_SEED to set the seed, and 
 * GSL_RNG_TYPE to set the random generator, e.g., ranlxs0, cmrg, mrg or taus
 * \see https://www.gnu.org/software/gsl/manual/html_node/Random-number-generator-algorithms.html#Random-number-generator-algorithms
 */
class RandomDraw
{
    /// Cache probability density per GraphElement
    std::map<GraphElementId, double> mProbabilityDensity;

    /// Marks if probability density has been updated or an element
    /// has been remove
    bool mUpdated;
    mutable double mProbabilityDensitySum;
    uint32_t mSeed;
    gsl_rng* mRandomNumberGenerator;

public:
    /**
     * \param seed If set to 0 will use a random seed, otherwise argument is
     * passed as seed to srand()
     */
    RandomDraw(uint32_t seed = 0);

    ~RandomDraw();

    /**
     * Set probability density
     * \param id GraphElementId for which the density should be set
     * \param density Density for the element id
     */
    void setProbabilityDensity(GraphElementId id, double density);

    /**
     * Get probability density
     * \return the overall sum of probability densities
     */
    double getProbabilityDensitySum() const;

    /**
     * Get the probability for drawing the given element
     * \return probability for drawing the element
     */
    double getProbability(GraphElementId id) const;

    /**
     * Remove an item
     */
    void removeItem(GraphElementId id);

    /**
     * Draw an item from the known set of elements
     * \return id of the drawn item
     */
    GraphElementId drawItem() const;
};

} // end namespace strategies
} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PERCOLATION_STRATEGIES_RANDOM_DRAW_HPP
