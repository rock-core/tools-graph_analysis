#ifndef GRAPH_ANALYSIS_FILTERS_REGEX_FILTER_HPP
#define GRAPH_ANALYSIS_FILTERS_REGEX_FILTER_HPP

#include <boost/regex.hpp>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/Edge.hpp>
#include <graph_analysis/filters/CommonFilters.hpp>
#include <base/Logging.hpp>

namespace graph_analysis {
namespace filters {

/**
 * RegexFilter provides the basic functionality to match Vertex or Edge content or class labels
 * using a regular expression
 */
template<typename T>
class RegexFilter : public graph_analysis::Filter<T>
{
protected:
    boost::regex mRegex;
    Type mType;
    bool mInverted;

public:
    RegexFilter(const std::string& regex, Type type, bool invert)
        : mRegex(regex)
        , mType(type)
        , mInverted(invert)
    {}

    virtual std::string getName() const { return "graph_analysis::filters::RegexFilter: '" + toString() + "'"; }

    virtual std::string toString() const
    {
        std::string txt = mRegex.str() + "/" + TypeTxt[mType] + "/";
        if(mInverted)
        {
            txt += "inverted";
        } else {
            txt += "not-inverted";
        }
        return txt;
    }

    virtual bool apply(T element) const
    {
        bool result = false;
        switch(mType)
        {
            case CONTENT:
                result = regex_match(element->toString(), mRegex);
                break;
            case CLASS:
                result = regex_match(element->getClassName(), mRegex);
                break;
            default:
                throw std::runtime_error("graph_analysis::filters::RegexFilter unknown filter type provided");
        }

        if(mInverted)
        {
            return !result;
        } else {
            return result;
        }
    }
};

/**
 * \class VertexRegexFilter
 * \brief Filter to sort out vertices
 */
class VertexRegexFilter : public RegexFilter< ::graph_analysis::Vertex::Ptr >
{
public:
    VertexRegexFilter(const std::string& regex, Type type = CONTENT, bool invert = false)
        : RegexFilter< ::graph_analysis::Vertex::Ptr>( regex, type, invert )
    {}

    virtual std::string getName() const { return "graph_analysis::filters::VertexRegexFilter: '" + toString() + "'"; }
};

/**
 * \class EdgeRegexFilter
 * \brief Filter to sort out edges
 */
class EdgeRegexFilter : public RegexFilter< ::graph_analysis::Edge::Ptr >
{
public:
    EdgeRegexFilter(const std::string& regex, Type type = CONTENT, bool invert = false)
        : RegexFilter< ::graph_analysis::Edge::Ptr>( regex, type, invert )
    {}

    virtual std::string getName() const { return "graph_analysis::filters::EdgeRegexFilter: '" + toString() + "'"; }
};

class CombinedEdgeRegexFilter : public EdgeRegexFilter
{
public:
    CombinedEdgeRegexFilter(VertexRegexFilter sourceNodeFilter, EdgeRegexFilter edgeFilter, VertexRegexFilter targetNodeFilter)
        : EdgeRegexFilter( edgeFilter )
        , mSourceNodeFilter(sourceNodeFilter)
        , mTargetNodeFilter(targetNodeFilter)
    {
    }

    virtual ~CombinedEdgeRegexFilter() {}

    virtual std::string getName() const { return "graph_analysis::filters::EdgeFilter: '" + toString(); }

    virtual std::string toString() const
    {
        std::string txt = "source node filter: " + mSourceNodeFilter.toString() + ", ";
        txt += "edge: " + EdgeRegexFilter::toString() + ", ";
        txt += "target node filter: " + mTargetNodeFilter.toString() + ", ";
        return txt;
    }

    virtual bool filterSource(graph_analysis::Edge::Ptr e) const
    {
        LOG_DEBUG_S << "FILTER source of edge";
        return mSourceNodeFilter.apply( e->getSourceVertex() );
    }

    virtual bool filterTarget(graph_analysis::Edge::Ptr e) const
    {
        LOG_DEBUG_S << "FILTER target of edge";
        return mTargetNodeFilter.apply( e->getTargetVertex() );
    }

private:
    VertexRegexFilter mSourceNodeFilter;
    VertexRegexFilter mTargetNodeFilter;
};

} // end namespace filters
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_FILTERS_REGEX_FILTER_HPP
