#include "MultiCommodityVertex.hpp"
#include "../VertexTypeManager.hpp"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace graph_analysis {
namespace algorithms {

const VertexRegistration<MultiCommodityVertex> MultiCommodityVertex::msRegistration;

MultiCommodityVertex::MultiCommodityVertex(const std::string& label)
    : Vertex(label)
{}

MultiCommodityVertex::MultiCommodityVertex(uint32_t commodities,
        const std::string& label)
    : Vertex(label)
    , mCommoditySupply(commodities,0)
    , mCommodityMinTransFlow(commodities, 0)
    , mCommodityMaxTransFlow(commodities, std::numeric_limits<uint32_t>::max())
    , mCombinedCommoditiesInFlowBounds()
{}

MultiCommodityVertex::~MultiCommodityVertex() {}

std::string MultiCommodityVertex::toString() const
{
    std::stringstream ss;
    {
        ss << getLabel() << "(" << getClassName() << ")" << " supply/demand: [";
        std::vector<int32_t>::const_iterator cit = mCommoditySupply.begin();
        for(; cit != mCommoditySupply.end();)
        {
            ss << *cit;
            if(++cit != mCommoditySupply.end())
            {
                ss << ",";
            }
        }
        ss << "]" << std::endl;
    }
    {
        ss << "         min-trans-flow: [";
        std::vector<uint32_t>::const_iterator cit = mCommodityMinTransFlow.begin();
        for(; cit != mCommodityMinTransFlow.end();)
        {
            ss << *cit;
            if(++cit != mCommodityMinTransFlow.end())
            {
                ss << ",";
            }
        }
        ss << "]";
    }
    {
        ss << "         max-trans-flow: [";
        std::vector<uint32_t>::const_iterator cit = mCommodityMaxTransFlow.begin();
        for(; cit != mCommodityMaxTransFlow.end();)
        {
            ss << *cit;
            if(++cit != mCommodityMaxTransFlow.end())
            {
                ss << ",";
            }
        }
        ss << "]";
    }
    return ss.str();
}

void MultiCommodityVertex::registerAttributes(VertexTypeManager* vManager) const
{
    vManager->registerAttribute( getClassName(), "vertex_attributes",
            (io::AttributeSerializationCallbacks::serialize_func_t) &MultiCommodityVertex::serializeAttributes,
            (io::AttributeSerializationCallbacks::deserialize_func_t) &MultiCommodityVertex::deserializeAttributes,
            (io::AttributeSerializationCallbacks::print_func_t) &MultiCommodityVertex::serializeAttributes);
}

std::string MultiCommodityVertex::serializeAttributes() const
{
    std::stringstream ss;
    boost::archive::text_oarchive oarch(ss);

    oarch << mCommoditySupply;
    oarch << mCommodityMinTransFlow;
    oarch << mCommodityMaxTransFlow;
    oarch << mCombinedCommoditiesInFlowBounds;

    return ss.str();
}

void MultiCommodityVertex::deserializeAttributes(const std::string& data)
{
    std::stringstream ss(data);
    boost::archive::text_iarchive iarch(ss);

    iarch >> mCommoditySupply;
    iarch >> mCommodityMinTransFlow;
    iarch >> mCommodityMaxTransFlow;
    iarch >> mCombinedCommoditiesInFlowBounds;
}

void MultiCommodityVertex::setCommoditiesMaxInFlow(const CommoditySet& commodities,
        uint32_t maxFlow)
{
    CombinedFlowBounds::iterator it = mCombinedCommoditiesInFlowBounds.find(commodities);
    if(it == mCombinedCommoditiesInFlowBounds.end())
    {
        mCombinedCommoditiesInFlowBounds.insert(
                CombinedFlowBounds::value_type(commodities,
                    std::pair<uint32_t,uint32_t>(0, maxFlow)));
    } else {
        it->second.second = maxFlow;
    }
}

void MultiCommodityVertex::setCommoditiesMinInFlow(const CommoditySet& commodities,
        uint32_t minFlow)
{
    CombinedFlowBounds::iterator it = mCombinedCommoditiesInFlowBounds.find(commodities);
    if(it == mCombinedCommoditiesInFlowBounds.end())
    {
        mCombinedCommoditiesInFlowBounds.insert(
                CombinedFlowBounds::value_type(commodities,
                    std::pair<uint32_t,uint32_t>(minFlow,
                        std::numeric_limits<uint32_t>::max())));
    } else {
        it->second.first = minFlow;
    }
}

} // end namespace algorithms
} // end namespace graph_analysis
