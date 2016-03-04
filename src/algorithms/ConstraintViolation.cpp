#include "ConstraintViolation.hpp"
#include <graph_analysis/MapInitializer.hpp>

namespace graph_analysis {
namespace algorithms {

std::map<ConstraintViolation::Type, std::string> ConstraintViolation::TypeTxt = InitMap<ConstraintViolation::Type, std::string>
    (MinFlow, "MinFlow")
    (TransFlow, "TransFlow");

ConstraintViolation::ConstraintViolation(const MultiCommodityVertex::Ptr& v, uint32_t commodity, int32_t delta, Type type)
    : mpVertex(v)
    , mCommodity(commodity)
    , mDelta(delta)
    , mType(type)
{}

std::string ConstraintViolation::toString() const
{
    std::stringstream ss;
    ss << "ConstraintViolation of " << TypeTxt[mType] << ":";
    ss << mpVertex->toString() << " commodity: " << mCommodity;
    ss << ", delta: " << mDelta;
    return ss.str();
}

} // end namespace algorithms
} // end namespace graph_analysis
