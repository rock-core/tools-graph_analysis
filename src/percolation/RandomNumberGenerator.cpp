#include "RandomNumberGenerator.hpp"
#include <base-logging/Logging.hpp>
#include <base/Time.hpp>

namespace graph_analysis {
namespace percolation {

RandomNumberGenerator::RandomNumberGenerator()
    : mRandomNumberGenerator(std::random_device()())
{
    base::Time time = base::Time::now();
    mRandomNumberGenerator.seed(time.microseconds);
}

RandomNumberGenerator::RandomNumberGenerator(uint32_t seed)
    : mRandomNumberGenerator(std::random_device()())
{
    mRandomNumberGenerator.seed(seed);
}

double RandomNumberGenerator::getUniformPositiveNumber() const
{
    // uniform real distribution between 0 and 1
    std::uniform_real_distribution<double> uniformRealDistribution(0,1) ;
    return uniformRealDistribution(mRandomNumberGenerator);
}

} // end namespace percolation
} // end namespace graph_analysis

