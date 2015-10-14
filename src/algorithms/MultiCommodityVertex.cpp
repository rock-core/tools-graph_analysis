#include "MultiCommodityVertex.hpp"

namespace graph_analysis {
namespace algorithms {

std::string MultiCommodityVertex::toString() const
{
    std::stringstream ss;
    ss << getClassName() << " supply/demand: [";
    std::vector<int32_t>::const_iterator cit = mCommoditySupply.begin();
    for(; cit != mCommoditySupply.end();)
    {
        ss << *cit;
        if(++cit != mCommoditySupply.end())
        {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}

} // end namespace algorithms
} // end namespace graph_analysis
