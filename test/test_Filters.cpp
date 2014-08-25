#include <boost/test/unit_test.hpp>
#include <graph_analysis/filters/RegexFilters.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_CASE(it_should_filter_with_regex)
{
    Vertex::Ptr v0( new Vertex());
    Vertex::Ptr v1( new Vertex());
    Edge::Ptr e0( new Edge());
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    {
        Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filters::PermitAll< Vertex::Ptr >() );
        BOOST_REQUIRE_MESSAGE( filter->evaluate(v0) == false, "Permit all filter should not apply any");

        filters::VertexRegexFilter::Ptr contentFilter(new filters::VertexRegexFilter(".*graph_analysis::.*", filters::CONTENT, false));
        filter->add(contentFilter);

        BOOST_REQUIRE_MESSAGE(filter->evaluate(v0), "Content filter: '" << contentFilter->getName() << "' should filter: " << v0->toString());
    }

    {
        Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filters::PermitAll< Vertex::Ptr >() );
        BOOST_REQUIRE_MESSAGE( filter->evaluate(v0) == false, "Permit all filter should not apply any");

        filters::VertexRegexFilter::Ptr contentFilter0(new filters::VertexRegexFilter(".*dummy.*", filters::CONTENT, false));
        filters::VertexRegexFilter::Ptr contentFilter1(new filters::VertexRegexFilter("graph_analysis.*", filters::CONTENT, false));
        filter->add(contentFilter0);
        filter->add(contentFilter1);

        BOOST_REQUIRE_MESSAGE(filter->evaluate(v0), "Content filter should filter");
    }
    {
        EdgeContextFilter::Ptr filter( new graph_analysis::filters::PermitAll< Edge::Ptr >());
        BOOST_REQUIRE_MESSAGE( filter->evaluate(e0) == false, "Permit all filter should not apply to any edge");
        BOOST_REQUIRE_MESSAGE( filter->filterTarget(e0) == false, "Permit all filter should not apply to any edge's target node");
        BOOST_REQUIRE_MESSAGE( filter->filterSource(e0) == false, "Permit all filter should not apply to any edge's source node");
    }

    {
        filters::EdgeRegexFilter edgeFilter;
        filters::VertexRegexFilter nodeFilter;

        Filter< Edge::Ptr >::Ptr filter(new filters::CombinedEdgeRegexFilter(nodeFilter, edgeFilter, nodeFilter));
        BOOST_REQUIRE_MESSAGE( !filter->evaluate(e0), "Edge filter should not filter");

        filters::CombinedEdgeRegexFilter::Ptr combinedFilter = boost::dynamic_pointer_cast<filters::CombinedEdgeRegexFilter>(filter);
        BOOST_REQUIRE_MESSAGE( !combinedFilter->filterTarget(e0), "Target vertex should not be filtered");
        BOOST_REQUIRE_MESSAGE( !combinedFilter->filterSource(e0), "Target vertex should not be filtered");
    }

    {
        filters::EdgeRegexFilter edgeFilter(".*");
        filters::VertexRegexFilter nodeFilter(".*");

        Filter< Edge::Ptr >::Ptr filter(new filters::CombinedEdgeRegexFilter(nodeFilter, edgeFilter, nodeFilter));
        BOOST_REQUIRE_MESSAGE( filter->evaluate(e0), "Edge filter should filter");

        EdgeContextFilter::Ptr combinedFilter = boost::dynamic_pointer_cast<filters::CombinedEdgeRegexFilter>(filter);
        BOOST_REQUIRE_MESSAGE( combinedFilter->filterTarget(e0), "Target vertex should be filtered");
        BOOST_REQUIRE_MESSAGE( combinedFilter->filterSource(e0), "Target vertex should be filtered");
    }

}
