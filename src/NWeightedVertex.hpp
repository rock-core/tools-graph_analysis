#ifndef GRAPH_ANALYSIS_NWEIGHTED_VERTEX_HPP
#define GRAPH_ANALYSIS_NWEIGHTED_VERTEX_HPP

#include <iostream>
#include "NWeighted.hpp"
#include "Vertex.hpp"
#include "VertexRegistration.hpp"

namespace graph_analysis {

/**
 * \brief NWeightedVertex is an Vertex which can be associated with N-weights, i.e.,
 * a vector of weights
 */
template<typename T, size_t Dim = 2>
class NWeightedVertex : public NWeighted<T,Dim,Vertex>
{
public:
    typedef shared_ptr< NWeightedVertex > Ptr;

    NWeightedVertex(const T& weight = T())
        : NWeighted<T,Dim,Vertex>(weight)
    {}

    virtual ~NWeightedVertex() {}

    void registerAttributes(VertexTypeManager* vManager) const override
    {
        vManager->registerAttribute(getClassName(), "weights",
           (io::AttributeSerializationCallbacks::serialize_func_t)&NWeightedVertex::serializeWeights,
           (io::AttributeSerializationCallbacks::deserialize_func_t)&NWeightedVertex::deserializeWeights,
           (io::AttributeSerializationCallbacks::print_func_t)&NWeightedVertex::stringifyWeights);
    }

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return
        "graph_analysis::NWeightedVertex_" + std::string(typeid(T).name()) + "_" + std::to_string(Dim); }

protected:
    virtual Vertex* getClone() const { return new NWeightedVertex<T,Dim>(*this); }

    static const VertexRegistration< NWeightedVertex<T,Dim> >  __attribute__((used)) msRegistration;
};

template<typename T, size_t Dim>
const VertexRegistration< NWeightedVertex<T,Dim> > NWeightedVertex<T,Dim>::msRegistration;

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_NWEIGHTED_VERTEX_HPP
