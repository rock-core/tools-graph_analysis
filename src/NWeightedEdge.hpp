#ifndef GRAPH_ANALYSIS_NWEIGHTED_EDGE_HPP
#define GRAPH_ANALYSIS_NWEIGHTED_EDGE_HPP

#include <iostream>
#include <graph_analysis/Edge.hpp>

namespace graph_analysis {

/**
 * \brief NWeightedEdge is an Edge which can be associated with N-weights, i.e.,
 * a vector of weights
 */
template<typename T, size_t Dim = 2>
class NWeightedEdge : public Edge
{
public:
    typedef boost::shared_ptr< NWeightedEdge > Ptr;

    NWeightedEdge(const std::vector<T>& weights = std::vector<T>(Dim))
        : Edge()
        , mWeights(weights)
    {
        validateDimensions(mWeights);
    }

    NWeightedEdge(Vertex::Ptr source, Vertex::Ptr target, const std::vector<T>& weights)
        : Edge(source, target)
        , mWeights(weights)
    {
        validateDimensions(mWeights);
    }

    virtual ~NWeightedEdge() {}

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::NWeightedEdge"; }

    /**
     * Convert element to string
     */
    virtual std::string toString() const
    {
        std::stringstream ss;
        ss << Edge::toString();
        ss << ": " << "[";
        typename std::vector<T>::const_iterator cit = mWeights.begin();
        for(;; ++cit)
        {
            ss << *cit;
            if(cit + 1 == mWeights.end())
            {
                break;
            } else {
                ss << ",";
            }
        }
        ss << "]";
        return ss.str();
    }

    void setWeights(const std::vector<T>& weights)
    {
        validateDimensions(weights);
        mWeights = weights;
    }

    std::vector<T> getWeights() const { return mWeights; }

    void setWeight(size_t index, T value)
    {
        try {
            mWeights.at(index) = value;
        } catch(const std::out_of_range& e)
        {
            throw std::out_of_range("graph_analysis::NWeightedEdge::getWeights: " + std::string(e.what()) );
        }
    }

    /**
     * Get weight at given index (dimension)
     */
    const T& getWeight(size_t index) const
    {
        try {
            return mWeights.at(index);
        } catch(const std::out_of_range& e)
        {
            throw std::out_of_range("graph_analysis::NWeightedEdge::getWeight: " + std::string(e.what()) );
        }
    }

protected:

    void validateDimensions(const std::vector<T>& weights) const
    {
        if(weights.size() != Dim)
        {
            std::stringstream ss;
            ss << "graph_analysis::NWeightedEdge: invalid number of weights: expected " << Dim << " got " << weights.size();
            throw std::invalid_argument(ss.str());
        }
    }

    virtual Edge* getClone() const { return new NWeightedEdge(*this); }

    std::vector<T> mWeights;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_NWEIGHTED_EDGE_HPP
