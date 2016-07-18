#include "RandomNumberGenerator.hpp"
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace percolation {

RandomNumberGenerator::RandomNumberGenerator(uint32_t seed)
{
    // https://www.gnu.org/software/gsl/manual/html_node/Random-number-generator-algorithms.html#Random-number-generator-algorithms

    // https://www.gnu.org/software/gsl/manual/html_node/Random-number-environment-variables.html#Random-number-environment-variables
    // via environment variable GSL_RNG_TYPE, default is gsl_rng_mt19937
    gsl_rng_env_setup();

    mRandomNumberGenerator = gsl_rng_alloc(gsl_rng_default);
    if(!seed)
    {
        // take the one from the environment / the default one
        mSeed = gsl_rng_default_seed;
        if(mSeed == 0)
        {
            mSeed = time(NULL);
        }
    } else {
        mSeed = seed;
    }
    gsl_rng_set(mRandomNumberGenerator, mSeed);

    LOG_DEBUG_S << "Using GSL random number generator: " << gsl_rng_name(mRandomNumberGenerator);
    LOG_DEBUG_S << "Using seed: " << mSeed;
}

RandomNumberGenerator::~RandomNumberGenerator()
{
    gsl_rng_free( mRandomNumberGenerator );
}

double RandomNumberGenerator::getUniformPositiveNumber() const
{
    return gsl_rng_uniform_pos(mRandomNumberGenerator);
}

} // end namespace percolation
} // end namespace graph_analysis

