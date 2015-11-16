#ifndef GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_VERTEX_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_VERTEX_HPP

#include <sstream>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {
namespace algorithms {

class MultiCommodityVertex : public Vertex
{
public:
    typedef shared_ptr<MultiCommodityVertex> Ptr;

    MultiCommodityVertex(uint32_t commodities)
        : mCommoditySupply(commodities,0)
    {}

    virtual ~MultiCommodityVertex() {}

    void setCommoditySupply(uint32_t commodity, int32_t supply) { mCommoditySupply.at(commodity) = supply; }

    int32_t getCommoditySupply(uint32_t commodity) const { return mCommoditySupply.at(commodity); }

    virtual std::string getClassName() const { return "MultiCommodityVertex"; }
    virtual std::string toString() const;

private:
    std::vector<int32_t> mCommoditySupply;
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_VERTEX_HPP
