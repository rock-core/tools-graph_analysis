#include "MultiCommodityEdge.hpp"
#include <sstream>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


namespace graph_analysis {
namespace algorithms {

const EdgeRegistration<MultiCommodityEdge> MultiCommodityEdge::msRegistration;

MultiCommodityEdge::MultiCommodityEdge()
{}

MultiCommodityEdge::MultiCommodityEdge(uint32_t commodities)
    : mCapacityUpperBound(0)
    , mCommodityCapacityUpperBound(commodities,0)
    , mCommodityCost(commodities,1)
    , mCommodityFlow(commodities,0)
{}

MultiCommodityEdge::~MultiCommodityEdge()
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

void MultiCommodityEdge::registerAttributes(EdgeTypeManager* eManager) const
{
    eManager->registerAttribute( getClassName(), "edge_attributes",
       (io::AttributeSerializationCallbacks::serialize_func_t)&MultiCommodityEdge::serializeAttributes,
       (io::AttributeSerializationCallbacks::deserialize_func_t)&MultiCommodityEdge::deserializeAttributes,
        (io::AttributeSerializationCallbacks::print_func_t)&MultiCommodityEdge::serializeAttributes);
}

std::string MultiCommodityEdge::serializeAttributes() const
{
    std::stringstream ss;
    boost::archive::text_oarchive oarch(ss);

    oarch << mCapacityUpperBound;
    oarch << mCommodityCapacityUpperBound;
    oarch << mCommodityCost;
    oarch << mCommodityFlow;

    return ss.str();
}

void MultiCommodityEdge::deserializeAttributes(const std::string& data)
{
    std::stringstream ss(data);
    boost::archive::text_iarchive iarch(ss);

    iarch >> mCapacityUpperBound;
    iarch >> mCommodityCapacityUpperBound;
    iarch >> mCommodityCost;
    iarch >> mCommodityFlow;
}


} // end namespace algorithms
} // end namespace graph_analysis
