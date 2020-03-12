#ifndef GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_VERTEX_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_VERTEX_HPP

#include <sstream>
#include "../Vertex.hpp"
#include "../VertexRegistration.hpp"

namespace graph_analysis {
namespace algorithms {

/**
 * \class MultiCommodityVertex
 * \brief A MultiCommodityVertex represents the combination of commodity supply
 * demand and a required transition flow
 *
 */
class MultiCommodityVertex : public Vertex
{
public:
    typedef shared_ptr<MultiCommodityVertex> Ptr;
    typedef std::set<uint32_t> CommoditySet;
    typedef std::map<CommoditySet, std::pair<uint32_t, uint32_t> > CombinedFlowBounds;

    MultiCommodityVertex(const std::string& label = "");

    MultiCommodityVertex(uint32_t commodities, const std::string& label = "");

    virtual ~MultiCommodityVertex();


    /**
     * Set the supply for a particular commodity
     * \param commodity Index of the commodity
     * \param supply (Integral) supply value of this commodity
     */
    void setCommoditySupply(uint32_t commodity, int32_t supply) { mCommoditySupply.at(commodity) = supply; }

    /**
     * Get the supply for a given commodity
     * \param commodity Index of the commodity
     */
    int32_t getCommoditySupply(uint32_t commodity) const { return mCommoditySupply.at(commodity); }

    /**
     * Set the inflow bounds (minimum/maximum) for a set of commodities, e.g.,
     * commodities of the same type
     * \param commodities Set of commodities
     * \param minMaxFlow Pair of lower and upper bound
     */
    void setCommoditiesInFlowBounds(const CommoditySet& commodities, std::pair<uint32_t,uint32_t> minMaxFlow) { mCombinedCommoditiesInFlowBounds[commodities] = minMaxFlow; }

    /**
     * Set the upper bound for the inflow of a set of commodities, e.g.,
     * commodities of the same type
     * \param commodities Set of commodities
     * \param maxFlow Maximum total inflow allowed for this set of commodities
     */
    void setCommoditiesMaxInFlow(const CommoditySet& commodities, uint32_t
            maxFlow);
    /**
     * Set the lower bound for the inflow of a set of commodities, e.g.,
     * commodities of the same type
     * \param commodities Set of commodities
     * \param minFlow Minimum total inflow required for this set of commodities
     */
    void setCommoditiesMinInFlow(const CommoditySet& commodities, uint32_t
            minFlow);

    /**
     * Get the combined maximum inflows for a set of commodities
     */
    const CombinedFlowBounds& getCommoditiesInFlowBounds() const { return mCombinedCommoditiesInFlowBounds; }

    /**
     * Request a trans-flow through this vertex for a given commodity
     * \param commodity Index of the commodity
     * \param flow (Integral) flow of the commodity
     */
    void setCommodityMinTransFlow(uint32_t commodity, uint32_t flow) { mCommodityMinTransFlow.at(commodity) = flow; }

    /**
     * Request a maximum trans-flow through this vertex for a given commodity
     * \param commodity Index of the commodity
     * \param flow (Integral) flow of the commodity
     */
    void setCommodityMaxTransFlow(uint32_t commodity, uint32_t flow) { mCommodityMaxTransFlow.at(commodity) = flow; }

    /**
     * Retrieve the set trans-flow through this vertex for a given commodity
     * \param commodity Index of the commodity
     * \return minimum trans-flow for the given commodity through this vertex
     */
    uint32_t getCommodityMinTransFlow(uint32_t commodity) const { return mCommodityMinTransFlow.at(commodity); }

    /**
     * Retrieve the set max trans-flow through this vertex for a given commodity
     * \param commodity Index of the commodity
     * \return minimum trans-flow for the given commodity through this vertex
     */
    uint32_t getCommodityMaxTransFlow(uint32_t commodity) const { return mCommodityMaxTransFlow.at(commodity); }

    virtual std::string getClassName() const override { return "MultiCommodityVertex"; }
    virtual std::string toString() const override;

    /**
     * Register attributes of this vertex for serialization
     */
    virtual void registerAttributes(VertexTypeManager*) const override;

private:
    std::vector<int32_t> mCommoditySupply;
    std::vector<uint32_t> mCommodityMinTransFlow;
    std::vector<uint32_t> mCommodityMaxTransFlow;
    /// General option to add inflow limits for groups of commodities
    CombinedFlowBounds mCombinedCommoditiesInFlowBounds;

    static const VertexRegistration<MultiCommodityVertex>  __attribute__((used)) msRegistration;

    virtual Vertex* getClone() const  { return new MultiCommodityVertex(*this); }

    std::string serializeAttributes() const;
    void deserializeAttributes(const std::string& data);
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_VERTEX_HPP
