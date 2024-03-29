#ifndef GRAPH_ANALYSIS_NWEIGHTED_EDGE_HPP
#define GRAPH_ANALYSIS_NWEIGHTED_EDGE_HPP

#include <iostream>

#include "NWeighted.hpp"
#include "Edge.hpp"
#include "EdgeRegistration.hpp"

namespace graph_analysis {

/**
 * \brief NWeightedEdge is an Edge which can be associated with N-weights, i.e.,
 * a vector of weights
 */
template<typename T, size_t Dim = 2>
class NWeightedEdge : public NWeighted<T,Dim,Edge>
{
public:
    typedef shared_ptr< NWeightedEdge > Ptr;

    NWeightedEdge(const T& weight)
        : NWeighted<T,Dim,Edge>(weight)
    {}

    NWeightedEdge(const std::vector<T>& weights = std::vector<T>(Dim))
        : NWeighted<T,Dim,Edge>(weights)
    {}

    NWeightedEdge(const Vertex::Ptr& source, const Vertex::Ptr& target, const T& weight)
        : NWeighted<T,Dim,Edge>(weight)
    {
        Edge::setSourceVertex(source);
        Edge::setTargetVertex(target);
    }

    NWeightedEdge(const Vertex::Ptr& source, const Vertex::Ptr& target, const std::vector<T>& weights)
        : NWeighted<T,Dim,Edge>(weights)
    {
        Edge::setSourceVertex(source);
        Edge::setTargetVertex(target);
    }

    void registerAttributes(EdgeTypeManager* eManager) const override
    {
        eManager->registerAttribute(getClassName(), "weights",
           (io::AttributeSerializationCallbacks::serialize_func_t)&NWeightedEdge::serializeWeights,
           (io::AttributeSerializationCallbacks::deserialize_func_t)&NWeightedEdge::deserializeWeights,
           (io::AttributeSerializationCallbacks::print_func_t)&NWeightedEdge::stringifyWeights);
    }

    virtual ~NWeightedEdge() {}

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return
        "graph_analysis::NWeightedEdge_" + std::string(typeid(T).name()) + "_" + std::to_string(Dim); }

protected:
    virtual Edge* getClone() const { return new NWeightedEdge<T,Dim>(*this); }

    static const EdgeRegistration< NWeightedEdge<T,Dim> >  __attribute__((used)) msRegistration;
};

template<typename T, size_t Dim>
const EdgeRegistration< NWeightedEdge<T,Dim> > NWeightedEdge<T,Dim>::msRegistration;

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_NWEIGHTED_EDGE_HPP
