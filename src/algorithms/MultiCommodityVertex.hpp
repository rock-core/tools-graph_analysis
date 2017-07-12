#ifndef GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_VERTEX_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_VERTEX_HPP

#include <sstream>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/VertexRegistration.hpp>

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

    MultiCommodityVertex();

    MultiCommodityVertex(uint32_t commodities);

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
     * Request a trans-flow through this vertex for a given commodity
     * \param commodity Index of the commodity
     * \param (Integral) flow of the commodity
     */
    void setCommodityMinTransFlow(uint32_t commodity, uint32_t flow) { mCommodityMinTransFlow.at(commodity) = flow; }

    /**
     * Retrieve the set trans-flow through this vertex for a given commodity
     * \param commodity Index of the commodity
     * \return minimum trans-flow for the given commodity through this vertex
     */
    uint32_t getCommodityMinTransFlow(uint32_t commodity) const { return mCommodityMinTransFlow.at(commodity); }

    virtual std::string getClassName() const override { return "MultiCommodityVertex"; }
    virtual std::string toString() const override;

    /**
     * Register attributes of this vertex for serialization
     */
    virtual void registerAttributes(VertexTypeManager*) const override;

private:
    std::vector<int32_t> mCommoditySupply;
    std::vector<uint32_t> mCommodityMinTransFlow;

    static const VertexRegistration<MultiCommodityVertex> msRegistration;

    virtual Vertex* getClone() const  { return new MultiCommodityVertex(*this); }

    std::string serializeAttributes() const;
    void deserializeAttributes(const std::string& data);
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_VERTEX_HPP
