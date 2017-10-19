#ifndef GRAPH_ANALYSIS_ALGORITHMS_CONSTRAINT_VIOLATION_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_CONSTRAINT_VIOLATION_HPP

#include <map>
#include <vector>
#include <graph_analysis/algorithms/MultiCommodityVertex.hpp>

namespace graph_analysis {
namespace algorithms {

class ConstraintViolation
{
public:
    enum Type { MinFlow, TransFlow, TotalMinFlow, TotalTransFlow };

    static std::map<Type, std::string> TypeTxt;

    ConstraintViolation(const MultiCommodityVertex::Ptr& v, uint32_t commodity, int32_t delta, Type type = MinFlow);

    /**
     * Construct a constraint violation with a set of commodities that are
     * affected by the constraint violation
     */
    ConstraintViolation(const MultiCommodityVertex::Ptr& v, const std::set<uint32_t>& commodities, int32_t delta, Type type = MinFlow);

    uint32_t getCommodity() const;

    /**
     * The set of commodities that are affected by this violation
     */
    const std::set<uint32_t>& getCommodities() const { return mCommodities; }

    /**
     * Get the vertex that this constraint violation is related to
     */
    const Vertex::Ptr& getVertex() const { return mpVertex; }

    /**
     * Quantification of the constraint violation, regarding the missing flow
     */
    int32_t getDelta() const { return mDelta; }

    /**
     * Get the constraint violation type
     */
    Type getType() const { return mType; }

    std::string toString(size_t indent = 0) const;

    static std::string toString(const std::vector<ConstraintViolation>& flaws, size_t indent = 0);

    static std::string toString(const std::set<uint32_t>& commodities);

private:
    Vertex::Ptr mpVertex;
    std::set<uint32_t> mCommodities;
    int32_t mDelta;
    Type mType;
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_CONSTRAINT_VIOLATION_HPP
