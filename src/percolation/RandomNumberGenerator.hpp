#ifndef GRAPH_ANALYSIS_PERCOLATION_RANDOM_NUMBER_GENERATOR_HPP
#define GRAPH_ANALYSIS_PERCOLATION_RANDOM_NUMBER_GENERATOR_HPP

#include <stdint.h>
#include <random>

namespace graph_analysis {
namespace percolation {

/**
 * \class RandomNumberGenerator
 * \details
 */
class RandomNumberGenerator
{
    uint32_t mSeed;
    mutable std::mt19937 mRandomNumberGenerator;

public:
    RandomNumberGenerator();

    RandomNumberGenerator(uint32_t seed);

    /**
     * Get a random number in the range (0,1) excluding both 0.0 and 1.0
     * \return random number in the range (0,1)
     */
    double getUniformPositiveNumber() const;
};

} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PERCOLATION_RANDOM_NUMBER_GENERATOR_HPP
