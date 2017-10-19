#include "ConstraintViolation.hpp"
#include <graph_analysis/MapInitializer.hpp>

namespace graph_analysis {
namespace algorithms {

std::map<ConstraintViolation::Type, std::string> ConstraintViolation::TypeTxt = InitMap<ConstraintViolation::Type, std::string>
    (MinFlow, "MinFlow")
    (TransFlow, "TransFlow")
    (TotalMinFlow, "TotalMinFlow")
    (TotalTransFlow, "TotalTransFlow")
    ;

ConstraintViolation::ConstraintViolation(const MultiCommodityVertex::Ptr& v, uint32_t commodity, int32_t delta, Type type)
    : mpVertex(v)
    , mDelta(delta)
    , mType(type)
{

    mCommodities.insert(commodity);
}

ConstraintViolation::ConstraintViolation(const MultiCommodityVertex::Ptr& v, const std::set<uint32_t>& commodities, int32_t delta, Type type)
    : mpVertex(v)
    , mCommodities(commodities)
    , mDelta(delta)
    , mType(type)
{}

std::string ConstraintViolation::toString(size_t indent) const
{
    std::string hspace(indent,' ');

    std::stringstream ss;
    ss << hspace << "ConstraintViolation of " << TypeTxt[mType] << ":" << std::endl;
    ss << mpVertex->toString(indent + 4)  << std::endl;
    ss << hspace << "    commodities: " << toString(mCommodities) << std::endl;
    ss << hspace << "    delta: " << mDelta << std::endl;
    return ss.str();
}

std::string ConstraintViolation::toString(const std::vector<ConstraintViolation>& flaws, size_t indent)
{
    std::stringstream ss;
    std::string hspace(indent,' ');
    ss << hspace << "ConstraintsViolations:" << std::endl;
    for(const ConstraintViolation& flaw : flaws)
    {
        ss << flaw.toString(indent + 4);
    }
    return ss.str();
}

std::string ConstraintViolation::toString(const std::set<uint32_t>& commodities)
{
    std::stringstream ss;
    ss << "[ ";
    for(uint32_t commodity : commodities)
    {
        ss << commodity << " ";
    }
    ss << "]";
    return ss.str();
}

uint32_t ConstraintViolation::getCommodity() const
{
    if(mCommodities.empty())
    {
        throw std::runtime_error("graph_analysis::algorithms::ConstraintViolation::getCommodity: no commodities set for constraint violation");
    } else if(mCommodities.size() == 1)
    {
        return *mCommodities.begin();
    } else {
        throw std::runtime_error("graph_analysis::algorithms::ConstraintViolation::getCommodity: multiple commodities available for constraint violation");
    }
}

} // end namespace algorithms
} // end namespace graph_analysis
