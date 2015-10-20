#include "MultiCommodityEdge.hpp"

namespace graph_analysis {
namespace algorithms {

MultiCommodityEdge::MultiCommodityEdge(uint32_t commodities)
    : mCapacityUpperBound(0)
    , mCommodityCapacityUpperBound(commodities,0)
    , mCommodityCost(commodities,1)
    , mCommodityFlow(commodities,0)
{}

void MultiCommodityEdge::setCommodityCapacityUpperBound(uint32_t commodity, uint32_t capacity)
{
    mCommodityCapacityUpperBound.at(commodity) = capacity;
}

std::string MultiCommodityEdge::toString() const
{
    std::stringstream ss;
    ss << getClassName() << ": UB:" << mCapacityUpperBound << std::endl;
    for(size_t i = 0; i < numberOfCommodities(); ++i)
    {
       ss << " #" << i << ": cost: " << mCommodityCost[i] << ", ub: " << mCommodityCapacityUpperBound[i]
           << ", flow: " << mCommodityFlow[i] << std::endl;
    }
    return ss.str();
}


} // end namespace algorithms
} // end namespace graph_analysis
