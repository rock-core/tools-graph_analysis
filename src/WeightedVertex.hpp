#ifndef GRAPH_ANALYSIS_WEIGHTED_VERTEX_HPP
#define GRAPH_ANALYSIS_WEIGHTED_VERTEX_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {

/**
 * \brief A WeightedVertex is an Vertex which can be associated with a weight
 */
class WeightedVertex : public Vertex
{
public:
    typedef boost::shared_ptr< WeightedVertex > Ptr;

    WeightedVertex(double weight = 0.0);

    virtual ~WeightedVertex() {}

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::WeightedVertex"; }

    /**
     * Convert element to string
     */
    virtual std::string toString() const;

    void setWeight(double weight) { mWeight = weight; }

    double getWeight() const { return mWeight; }

protected:
    virtual Vertex* getClone() const { return new WeightedVertex(*this); }

    double mWeight;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_WEIGHTED_VERTEX_HPP
