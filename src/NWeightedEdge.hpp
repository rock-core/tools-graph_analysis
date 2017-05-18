#ifndef GRAPH_ANALYSIS_NWEIGHTED_EDGE_HPP
#define GRAPH_ANALYSIS_NWEIGHTED_EDGE_HPP

#include <iostream>
#include <graph_analysis/NWeighted.hpp>
#include <graph_analysis/Edge.hpp>

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

    virtual ~NWeightedEdge() {}

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::NWeightedEdge"; }

protected:
    virtual Edge* getClone() const { return new NWeightedEdge<T,Dim>(*this); }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_NWEIGHTED_EDGE_HPP
