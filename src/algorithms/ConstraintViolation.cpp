#include "ConstraintViolation.hpp"
#include <boost/assign/list_of.hpp>

namespace graph_analysis {
namespace algorithms {

std::map<ConstraintViolation::Type, std::string> ConstraintViolation::TypeTxt = boost::assign::map_list_of
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
    ss << "ConstraintViolation: " << mpVertex->toString() << " commodity: " << mCommodity
        << ", delta: " << mDelta << ", type: " << (int) mType;
    return ss.str();
}

} // end namespace algorithms
} // end namespace graph_analysis
