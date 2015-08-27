#ifndef GRAPH_ANALYSIS_NWEIGHTED_VERTEX_HPP
#define GRAPH_ANALYSIS_NWEIGHTED_VERTEX_HPP

#include <iostream>
#include <graph_analysis/NWeighted.hpp>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {

/**
 * \brief NWeightedVertex is an Vertex which can be associated with N-weights, i.e.,
 * a vector of weights
 */
template<typename T, size_t Dim = 2>
class NWeightedVertex : public NWeighted<T,Dim,Vertex>
{
public:
    typedef boost::shared_ptr< NWeightedVertex > Ptr;

    NWeightedVertex(const T& weight)
        : NWeighted<T,Dim,Vertex>(weight)
    {}

    virtual ~NWeightedVertex() {}

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::NWeightedVertex"; }

protected:
    virtual NWeightedVertex<T,Dim>* getClone() const { return new NWeightedVertex<T,Dim>(*this); }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_NWEIGHTED_VERTEX_HPP
