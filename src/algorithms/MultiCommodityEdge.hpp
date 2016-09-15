#ifndef GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_EDGE_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_EDGE_HPP

#include <graph_analysis/Edge.hpp>

namespace graph_analysis {
namespace algorithms {

class MultiCommodityEdge : public Edge
{
public:
    typedef shared_ptr<MultiCommodityEdge> Ptr;

    MultiCommodityEdge(uint32_t commodities);
    virtual ~MultiCommodityEdge() {}

    virtual std::string getClassName() const { return "MultiCommodityEdge"; }

    virtual std::string toString() const;

    /**
     * Set the general capacity bound for this edge
     * sum of all flows on this edge cannot exceed this bound
     */
    void setCapacityUpperBound(uint32_t capacity) { mCapacityUpperBound = capacity; }
    uint32_t getCapacityUpperBound() const { return mCapacityUpperBound; }

    /**
     * Set the commodity specific bound for this edge, i.e.
     * the flow of a commodity cannot exceed this bound across this edge
     */
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

    size_t numberOfCommodities() const { return mCommodityFlow.size(); }

private:
    uint32_t mCapacityUpperBound;
    std::vector<uint32_t> mCommodityCapacityUpperBound;
    std::vector<double> mCommodityCost;

    // Result
    std::vector<uint32_t> mCommodityFlow;

    virtual Edge* getClone() const  { return new MultiCommodityEdge(*this); }
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_EDGE_HPP
