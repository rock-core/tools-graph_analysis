#include "RandomDraw.hpp"
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <base-logging/Logging.hpp>
#include <graph_analysis/percolation/RandomNumberGenerator.hpp>

namespace graph_analysis {
namespace percolation {
namespace strategies {

RandomDraw::RandomDraw(uint32_t seed)
    : mUpdated(false)
    , mProbabilityDensitySum(0.0)
    , mRandomNumberGenerator(new RandomNumberGenerator(seed))
{
}

RandomDraw::~RandomDraw()
{
    delete mRandomNumberGenerator;
}

void RandomDraw::setProbabilityDensity(GraphElementId id, double density)
{
    mProbabilityDensity[id] = density;
    mUpdated = true;
}

double RandomDraw::getProbabilityDensitySum() const
{
    if(!mUpdated)
    {
        return mProbabilityDensitySum;
    }

    mProbabilityDensitySum = 0;
    std::map<GraphElementId, double>::const_iterator cit = mProbabilityDensity.begin();
    for(; cit != mProbabilityDensity.end(); ++cit)
    {
        mProbabilityDensitySum += cit->second;
    }
    return mProbabilityDensitySum;
}

double RandomDraw::getProbability(GraphElementId id) const
{
    std::map<GraphElementId, double>::const_iterator cit = mProbabilityDensity.find(id);
    double probabilityDensity = 0.0;
    if(cit == mProbabilityDensity.end())
    {
        std::stringstream ss;
        ss << id;
        throw std::invalid_argument("graph_analysis::percolation::strategies::RandomDraw::getProbability: no graph element with id " + ss.str());
    } else {
        probabilityDensity = cit->second;
    }

    double probabilityDensitySum = getProbabilityDensitySum();
    if(probabilityDensitySum != 0)
    {
        return probabilityDensity / probabilityDensitySum;
    } else {
        return 0.0;
    }
}

void RandomDraw::removeItem(GraphElementId id)
{
    std::map<GraphElementId, double>::iterator it = mProbabilityDensity.find(id);
    if(it != mProbabilityDensity.end())
    {
        mProbabilityDensity.erase(it);
        mUpdated = true;
    } else {
        std::stringstream idStr;
        idStr << id;

        throw std::invalid_argument("graph_analysis::percolation::strategies::RandomDraw::remoteItem element with id '" + idStr.str() + "' does not exist");
    }
}

GraphElementId RandomDraw::drawItem() const
{
    if(mProbabilityDensity.empty())
    {
        throw std::runtime_error("graph_analysis::percolation::strategies::RandomDraw::drawItem: could not draw any item. Not elements provided");
    }

    double probabilityDensitySum = getProbabilityDensitySum();
    if(probabilityDensitySum == 0)
    {
        throw std::runtime_error("graph_analysis::percolation::strategies::RandomDraw::drawItem: could not draw any item, probabilityDensitySum is 0");
    }

    // Get the random number in the range (0,1) excluding both 0.0 and 1.0
    double randomNumber = mRandomNumberGenerator->getUniformPositiveNumber();
    randomNumber *= probabilityDensitySum;

    double currentDensity = 0.0;
    std::map<GraphElementId, double>::const_iterator cit = mProbabilityDensity.begin();
    for(; cit != mProbabilityDensity.end(); ++cit)
    {
        currentDensity += cit->second;
        if(randomNumber <= currentDensity)
        {
            return cit->first;
        }
    }

    std::stringstream randomNumberStr;
    randomNumberStr << randomNumber;

    std::stringstream probabilityDensitySumStr;
    probabilityDensitySumStr << mProbabilityDensitySum;

    throw std::runtime_error("graph_analysis::percolation::strategies::RandomDraw::drawItem: could not draw any item: randomNumber: '" + randomNumberStr.str() + "', probability density sum: '" + probabilityDensitySumStr.str() + "'");
}

} // end namespace strategies
} // end namespace percolation
} // end namespace graph_analysis
