#ifndef GRAPH_ANALYSIS_PERCOLATION_RANDOM_NUMBER_GENERATOR_HPP
#define GRAPH_ANALYSIS_PERCOLATION_RANDOM_NUMBER_GENERATOR_HPP

#include <stdint.h>
#include <gsl/gsl_rng.h>

namespace graph_analysis {
namespace percolation {

/**
 * \class RandomNumberGenerator
 * \details
 * You can use the environoment variable GSL_RNG_SEED to set the seed, and 
 * GSL_RNG_TYPE to set the random generator, e.g., ranlxs0, cmrg, mrg or taus
 * \see https://www.gnu.org/software/gsl/manual/html_node/Random-number-generator-algorithms.html#Random-number-generator-algorithms
 */
class RandomNumberGenerator
{
    uint32_t mSeed;
    gsl_rng* mRandomNumberGenerator;

public:
    RandomNumberGenerator(uint32_t seed = 0);

    ~RandomNumberGenerator();

    /**
     * Get a random number in the range (0,1) excluding both 0.0 and 1.0
     * \return random number in the range (0,1)
     */
    double getUniformPositiveNumber() const;
};

} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PERCOLATION_RANDOM_NUMBER_GENERATOR_HPP
