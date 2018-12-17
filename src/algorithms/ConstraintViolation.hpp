#ifndef GRAPH_ANALYSIS_ALGORITHMS_CONSTRAINT_VIOLATION_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_CONSTRAINT_VIOLATION_HPP

#include <map>
#include <vector>
#include "MultiCommodityVertex.hpp"
#include "MultiCommodityEdge.hpp"

namespace graph_analysis {
namespace algorithms {

/**
  * \brief Representation of ConstraintViolation in a min-cost flow problem
  * \details To identify the known violations of constraints, this class permits
  * to represent constraint violations. A quantification of the violation is
  * part of this representation.
  */
class ConstraintViolation
{
public:
    enum Type {
        /// Inflow does not equal Outflow
        FlowBalance,
        /// The minimum required flow (for a single commodity) is not available
        MinFlow,
        /// The required transflow (min inflow + min outflow) for a single
        /// commodity is not available
        TransFlow,
        /// The required minimum flow for all commodities related to this node
        /// is not satisfied
        TotalMinFlow,
        /// The required transflow for all commodities related to this node
        /// is not satisfied
        TotalTransFlow
    };

    /// The string representation for each type
    static std::map<Type, std::string> TypeTxt;

    /**
      * Default Constructor
      * \param v The vertex where the constraint violation is encountered
      * \param commodity The commodity for which the constraint is violated
      * \param delta The difference to the actually required fulfilment
      * \param inFlow Current inflow
      * \param outFlow Current outflow
      * \param type Classification of violation
      */
    ConstraintViolation(const MultiCommodityVertex::Ptr& v, uint32_t commodity, int32_t delta, uint32_t inFlow, uint32_t outFlow, Type type = MinFlow);

    /**
      * Default Constructor
      * \param e The edge where the constraint violation is encountered
      * \param commodity The commodity for which the constraint is violated
      * \param delta The difference to the actually required fulfilment
      * \param inFlow Current inflow
      * \param outFlow Current outflow
      * \param type Classification of violation
      */
    ConstraintViolation(const MultiCommodityEdge::Ptr& e, uint32_t commodity, int32_t delta, uint32_t inFlow, uint32_t outFlow, Type type = MinFlow);

    /**
      * Construct a constraint violation with a set of commodities that are
      * affected by the constraint violation
      * \param v The vertex where the constraint violation is encountered
      * \param commodities The set of commodities for which the constraint is violated
      * \param delta The difference to the actually required fulfilment
      * \param inFlow Current inflow
      * \param outFlow Current outflow
      * \param type Classification of violation
      */
    ConstraintViolation(const MultiCommodityVertex::Ptr& v, const std::set<uint32_t>& commodities, int32_t delta, uint32_t inFlow, uint32_t outFlow, Type type = MinFlow);

    /**
      * Construct a constraint violation with a set of commodities that are
      * affected by the constraint violation
      * \param e The edge where the constraint violation is encountered
      * \param commodities The set of commodities for which the constraint is violated
      * \param delta The difference to the actually required fulfilment
      * \param inFlow Current inflow
      * \param outFlow Current outflow
      * \param type Classification of violation
      */
    ConstraintViolation(const MultiCommodityEdge::Ptr& e, const std::set<uint32_t>& commodities, int32_t delta, uint32_t inFlow, uint32_t outFlow, Type type = MinFlow);

    /**
      * Get the commodity id
      * \return commodity id
      */
    uint32_t getCommodity() const;

    /**
      * Get the current inflow
      * \return inflow
      */
    uint32_t getInFlow() const { return mInFlow; }

    /**
      * Get the current outflow
      * \return outflow
      */
    uint32_t getOutFlow() const { return mOutFlow; }

    /**
     * The set of commodities that are affected by this violation
     * \return set of commodities
     */
    const std::set<uint32_t>& getCommodities() const { return mCommodities; }

    /**
     * Get the vertex that this constraint violation is related to
     * (can be null)
     * \return affected vertex
     */
    const Vertex::Ptr& getVertex() const { return mpVertex; }

    /**
     * Get the edge that this constraint violation is related to
     * (can be null)
     * \return affected edge
     */
    const Edge::Ptr& getEdge() const { return mpEdge; }

    /**
     * Quantification of the constraint violation, regarding the missing flow
     * \return missing flow
     */
    int32_t getDelta() const { return mDelta; }

    /**
     * Get the constraint violation type
     * \return type
     */
    Type getType() const { return mType; }

    /**
      * Stringify constraint
      * \param indent Number of indentation spaces
      * \return string object
      */
    std::string toString(size_t indent = 0) const;

    /**
      * Stringify list of constraint violations
      * \return string object
      */
    static std::string toString(const std::vector<ConstraintViolation>& flaws, size_t indent = 0);

    /**
      * Stringify commodities
      * \return string object
      */
    static std::string toString(const std::set<uint32_t>& commodities);

private:
    Vertex::Ptr mpVertex;
    Edge::Ptr mpEdge;
    std::set<uint32_t> mCommodities;
    int32_t mDelta;
    uint32_t mInFlow;
    uint32_t mOutFlow;
    Type mType;
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_CONSTRAINT_VIOLATION_HPP
