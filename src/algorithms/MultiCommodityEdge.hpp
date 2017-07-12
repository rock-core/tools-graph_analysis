#ifndef GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_EDGE_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_EDGE_HPP

#include <graph_analysis/Edge.hpp>
#include <graph_analysis/EdgeRegistration.hpp>

namespace graph_analysis {
namespace algorithms {

/**
 * \class MultiCommodityEdge
 * \brief Representing of an edge in a multicommodity flow problem
 */
class MultiCommodityEdge : public Edge
{
public:
    typedef shared_ptr<MultiCommodityEdge> Ptr;

    /**
     * Default constructor to support serialization
     */
    MultiCommodityEdge();

    /**
     * Construct edge with a given number of commodities which can go through
     * this edge
     */
    MultiCommodityEdge(uint32_t commodities);

    /**
     * Deconstructor
     */
    virtual ~MultiCommodityEdge();

    /**
     * The class name of this instance
     */
    virtual std::string getClassName() const { return "MultiCommodityEdge"; }

    /**
     * Provide a string representation of this object
     */
    virtual std::string toString() const;

    /**
     * Set the general capacity bound for this edge
     * sum of all flows on this edge cannot exceed this bound
     */
    void setCapacityUpperBound(uint32_t capacity) { mCapacityUpperBound = capacity; }

    /***
     * Get the general capacity bound for this edge
     * \return Upper capacity bound
     */
    uint32_t getCapacityUpperBound() const { return mCapacityUpperBound; }

    /**
     * Set the commodity specific bound for this edge, i.e.
     * the flow of a commodity cannot exceed this bound across this edge
     * \param commodity Index of the commodity
     * \param capacity Capacity limit for this commodity
     */
    void setCommodityCapacityUpperBound(uint32_t commodity, uint32_t capacity);

    /**
     * Get the commodity specific bound for this edge
     * \param commodity Index of the commodity
     */
    uint32_t getCommodityCapacityUpperBound(uint32_t commodity) const { return mCommodityCapacityUpperBound.at(commodity); }

    /**
     * Set the cost for a particular commodity
     * \param commodity Index of the commodity
     * \param cost Cost for this commodity
     */
    void setCommodityCost(uint32_t commodity, double cost) { mCommodityCost.at(commodity) = cost; }

    /**
     * Get the cost of the commodity with the given index
     * \param commodity Index of the commodity
     */
    double getCommodityCost(uint32_t commodity) { return mCommodityCost.at(commodity); }

    /**
     * Set the commodity flow (after a solution has been computed)
     * \param commodity Index of the commodity
     * \param flow (Integral) flow of this commodity
     */
    void setCommodityFlow(uint32_t commodity, uint32_t flow) { mCommodityFlow.at(commodity) = flow; }

    /**
     * Get the assigned commodity flow
     * \return flow assigned to the commodity with the given index
     */
    uint32_t getCommodityFlow(uint32_t commodity) const { return mCommodityFlow.at(commodity); }

    /**
     * Get the number of commodities in this optimization instance
     */
    size_t numberOfCommodities() const { return mCommodityFlow.size(); }

    /**
     * Register attributes for serialization
     */
    virtual void registerAttributes(EdgeTypeManager*) const;

private:
    /// Upper bound on the overall capacity
    uint32_t mCapacityUpperBound;
    /// Upper bound on individual capacities (per commodity)
    std::vector<uint32_t> mCommodityCapacityUpperBound;
    /// Costs of commodities
    std::vector<double> mCommodityCost;

    /// Resulting flow for this commodity (for this edge)
    std::vector<uint32_t> mCommodityFlow;

    /// Perform registration for serialization
    static const EdgeRegistration<MultiCommodityEdge> msRegistration;

    virtual Edge* getClone() const  { return new MultiCommodityEdge(*this); }

    /**
     * Serialize all attributes of this edge
     */
    std::string serializeAttributes() const;

    /**
     * Restore attributes of this edge from given serialized data
     */
    void deserializeAttributes(const std::string& data);

};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_EDGE_HPP
