#ifndef GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_EDGE_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_EDGE_HPP

#include <graph_analysis/Edge.hpp>

namespace graph_analysis {
namespace algorithms {

class MultiCommodityEdge : public Edge
{
public:
    typedef boost::shared_ptr<MultiCommodityEdge> Ptr;

    MultiCommodityEdge(uint32_t commodities);
    virtual ~MultiCommodityEdge() {}

    void setCapacityUpperBound(uint32_t capacity) { mCapacityUpperBound = capacity; }
    uint32_t getCapacityUpperBound() const { return mCapacityUpperBound; }

    void setCommodityCapacityUpperBound(uint32_t commodity, uint32_t capacity);
    uint32_t getCommodityCapacityUpperBound(uint32_t commodity) const { return mCommodityCapacityUpperBound.at(commodity); }

    void setCommodityCost(uint32_t commodity, double cost) { mCommodityCost.at(commodity) = cost; }
    double getCommodityCost(uint32_t commodity) { return mCommodityCost.at(commodity); }

    /**
     * Set the commodity flow (after a solution has been computed)
     */
    void setCommodityFlow(uint32_t commodity, uint32_t flow) { mCommodityFlow.at(commodity) = flow; }

    /**
     * Get the assigned commodity flow
     */
    uint32_t getCommodityFlow(uint32_t commodity) const { return mCommodityFlow.at(commodity); }

private:
    uint32_t mCapacityUpperBound;
    std::vector<uint32_t> mCommodityCapacityUpperBound;
    std::vector<double> mCommodityCost;

    // Result
    std::vector<uint32_t> mCommodityFlow;
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_EDGE_HPP
