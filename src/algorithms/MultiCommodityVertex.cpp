#include "MultiCommodityVertex.hpp"
#include "../VertexTypeManager.hpp"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace graph_analysis {
namespace algorithms {

const VertexRegistration<MultiCommodityVertex> MultiCommodityVertex::msRegistration;

MultiCommodityVertex::MultiCommodityVertex()
{}

MultiCommodityVertex::MultiCommodityVertex(uint32_t commodities)
    : mCommoditySupply(commodities,0)
    , mCommodityMinTransFlow(commodities, 0)
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

    return ss.str();
}

void MultiCommodityVertex::deserializeAttributes(const std::string& data)
{
    std::stringstream ss(data);
    boost::archive::text_iarchive iarch(ss);

    iarch >> mCommoditySupply;
    iarch >> mCommodityMinTransFlow;
}

} // end namespace algorithms
} // end namespace graph_analysis
