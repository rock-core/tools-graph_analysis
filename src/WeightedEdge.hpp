#ifndef GRAPH_ANALYSIS_WEIGHTED_EDGE_HPP
#define GRAPH_ANALYSIS_WEIGHTED_EDGE_HPP

#include <iostream>
#include <graph_analysis/Edge.hpp>

namespace graph_analysis {

/**
 * \brief A WeightedEdge is an Edge which can be associated with a weight
 */
class WeightedEdge : public Edge
{
public:
    typedef boost::shared_ptr< WeightedEdge > Ptr;

    WeightedEdge(double weight = 0.0);

    WeightedEdge(Vertex::Ptr source, Vertex::Ptr target, double weight = 0.0);

    virtual ~WeightedEdge() {}

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::WeightedEdge"; }

    /**
     * Convert element to string
     */
    virtual std::string toString() const;

    void setWeight(double weight) { mWeight = weight; }

    double getWeight() const { return mWeight; }

protected:
    virtual Edge* getClone() const { return new WeightedEdge(*this); }

    double mWeight;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_WEIGHTED_EDGE_HPP
