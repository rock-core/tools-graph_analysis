#include "RegexFilters.hpp"

namespace graph_analysis {
namespace filters {

VertexRegexFilter::VertexRegexFilter(const std::string& regex, Type type, bool invert)
    : RegexFilter< ::graph_analysis::Vertex::Ptr>( regex, type, invert )
{}

std::string VertexRegexFilter::getName() const { return "graph_analysis::filters::VertexRegexFilter: '" + toString() + "'"; }

EdgeRegexFilter::EdgeRegexFilter(const std::string& regex, Type type, bool invert)
    : RegexFilter< ::graph_analysis::Edge::Ptr>( regex, type, invert )
{}

std::string EdgeRegexFilter::getName() const { return "graph_analysis::filters::EdgeRegexFilter: '" + toString() + "'"; }

CombinedEdgeRegexFilter::CombinedEdgeRegexFilter(VertexRegexFilter sourceNodeFilter, EdgeRegexFilter edgeFilter, VertexRegexFilter targetNodeFilter)
    : mEdgeRegexFilter(edgeFilter)
    , mSourceNodeFilter(sourceNodeFilter)
    , mTargetNodeFilter(targetNodeFilter)
{
    Filter<Edge::Ptr>::Ptr filter(new EdgeRegexFilter(edgeFilter));
    add(filter);
}

CombinedEdgeRegexFilter::~CombinedEdgeRegexFilter() {}

std::string CombinedEdgeRegexFilter::getName() const { return "graph_analysis::filters::CombindedEdgeRegexFilter: '" + toString(); }

std::string CombinedEdgeRegexFilter::toString() const
{
    std::string txt = "source node filter: " + mSourceNodeFilter.toString() + ", ";
    txt += "edge: " + mEdgeRegexFilter.toString() + ", ";
    txt += "target node filter: " + mTargetNodeFilter.toString() + ", ";
    return txt;
}

bool CombinedEdgeRegexFilter::evaluateSource(graph_analysis::Edge::Ptr e) const
{
    return mSourceNodeFilter.apply( e->getSourceVertex() );
}

bool CombinedEdgeRegexFilter::evaluateTarget(graph_analysis::Edge::Ptr e) const
{
    return mTargetNodeFilter.apply( e->getTargetVertex() );
}

} // end namespace filters
} // end namespace graph_analysis
