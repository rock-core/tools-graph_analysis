#ifndef GRAPH_ANALYSIS_NWEIGHTED_HPP
#define GRAPH_ANALYSIS_NWEIGHTED_HPP

#include <sstream>
#include <stdexcept>
#include <vector>
#include <iomanip>

#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "SharedPtr.hpp"

namespace graph_analysis {

/**
 * \brief NWeighted is a template to associate N-weights with either a vertex
 * or an edge, i.e., adds a vector of weights
 *
 * \tparam T value type of the weights
 * \tparam Dim dimensions / number of weights
 * \tparam GraphElementType, used here for either Vertex or Edge
 */
template<typename T, size_t Dim, typename GraphElementType>
class NWeighted : public GraphElementType
{
public:
    typedef shared_ptr< NWeighted > Ptr;
    typedef T value_t;

    /**
     * Default constructor for NWeighted to support single weight initialization
     * This sets the weight at index 0
     */
    NWeighted(const T& weight)
        : GraphElementType()
        , mWeights(Dim)
    {
        setWeight(weight, 0);
        validateDimensions(mWeights);
    }

    /**
     * Default constructor for NWeighted to support multi-weight initialization
     */
    NWeighted(const std::vector<T>& weights = std::vector<T>(Dim))
        : GraphElementType()
        , mWeights(weights)
    {
        validateDimensions(mWeights);
    }

    NWeighted(const NWeighted& other)
    {
        mWeights = other.mWeights;
    }

    virtual ~NWeighted() {}

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::NWeighted"; }

    /**
     * Convert element to string
     */
    virtual std::string toString() const
    {
        std::stringstream ss;
        ss << GraphElementType::toString();
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

    std::vector<T> getWeights() const
    {
        return mWeights;
    }

    void setWeight(T value, size_t index = 0)
    {
        try {
            mWeights.at(index) = value;
        } catch(const std::out_of_range& e)
        {
            std::stringstream ss;
            ss << "graph_analysis::NWeighted::setWeights: ";
            ss << " dim: is " << Dim << ", tried access at: " << index;
            ss << ": " << std::string(e.what());
            throw std::out_of_range(ss.str());
        }
    }

    /**
     * Get weight at given index (dimension)
     */
    const T& getWeight(size_t index = 0) const
    {
        try {
            return mWeights.at(index);
        } catch(const std::out_of_range& e)
        {
            throw std::out_of_range("graph_analysis::NWeighted::getWeight: " + std::string(e.what()) );
        }
    }

    std::string serializeWeights()
    {
        std::stringstream ss;
        boost::archive::text_oarchive oarch(ss);
        oarch << mWeights;
        return ss.str();
    }

    void deserializeWeights(const std::string& s)
    {
        std::stringstream ss;
        ss << s;
        boost::archive::text_iarchive iarch(ss);
        iarch >> mWeights;
    }

    std::string stringifyWeights() const
    {
        std::stringstream ss;
        ss << "[";
        bool first = true;
        for(const T& w : mWeights)
        {
            if(first)
            {
                first = false;
            } else {
                ss << ",";
            }
            ss << w;
        }
        ss << "]";
        return ss.str();
    }

protected:

    void validateDimensions(const std::vector<T>& weights) const
    {
        if(weights.size() != Dim)
        {
            std::stringstream ss;
            ss << "graph_analysis::NWeighted: invalid number of weights: expected " << Dim << " got " << weights.size();
            throw std::invalid_argument(ss.str());
        }
    }

    virtual GraphElementType* getClone() const { return new NWeighted(*this); }

    std::vector<T> mWeights;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_NWEIGHTED_VERTEX_HPP
