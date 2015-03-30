#include <boost/test/unit_test.hpp>
#include <graph_analysis/filters/RegexFilters.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(filters_test)

BOOST_AUTO_TEST_CASE(it_should_filter_with_regex)
{
    Vertex::Ptr v0( new Vertex());
    Vertex::Ptr v1( new Vertex());
    Edge::Ptr e0( new Edge());
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    {
        Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filters::PermitAll< Vertex::Ptr >() );
        BOOST_REQUIRE_MESSAGE( filter->permits(v0) == true, "Permit all filter should permit all");

        filters::VertexRegexFilter::Ptr contentFilter(new filters::VertexRegexFilter(".*graph_analysis::.*", filters::CONTENT, false));
        filter->add(contentFilter);

        BOOST_REQUIRE_MESSAGE(filter->permits(v0), "Content filter: '" << contentFilter->getName() << "' should filter: " << v0->toString());
    }

    {
        Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filters::PermitAll< Vertex::Ptr >() );
        BOOST_REQUIRE_MESSAGE( filter->permits(v0) == true, "Permit all filter should permit all");

        filters::VertexRegexFilter::Ptr contentFilter0(new filters::VertexRegexFilter(".*dummy.*", filters::CONTENT, false));
        filters::VertexRegexFilter::Ptr contentFilter1(new filters::VertexRegexFilter("graph_analysis.*", filters::CONTENT, false));
        filter->add(contentFilter0);
        filter->add(contentFilter1);

        BOOST_REQUIRE_MESSAGE(filter->permits(v0), "Content filter should permit");
    }
    {
        EdgeContextFilter::Ptr filter( new graph_analysis::filters::PermitAll< Edge::Ptr >());
        BOOST_REQUIRE_MESSAGE( filter->permits(e0) == true, "Permit all filter should no apply to any edge");
        BOOST_REQUIRE_MESSAGE( filter->permitsTarget(e0) == true, "Permit all filter should apply to any edge's target node");
        BOOST_REQUIRE_MESSAGE( filter->permitsSource(e0) == true, "Permit all filter should apply to any edge's source node");
    }

    {
        filters::EdgeRegexFilter edgeFilter;
        filters::VertexRegexFilter nodeFilter;

        Filter< Edge::Ptr >::Ptr filter(new filters::CombinedEdgeRegexFilter(nodeFilter, edgeFilter, nodeFilter));
        BOOST_REQUIRE_MESSAGE( !filter->permits(e0), "Edge filter should not permit");

        filters::CombinedEdgeRegexFilter::Ptr combinedFilter = boost::dynamic_pointer_cast<filters::CombinedEdgeRegexFilter>(filter);
        BOOST_REQUIRE_MESSAGE( !combinedFilter->permitsTarget(e0), "Target vertex should not be permitted");
        BOOST_REQUIRE_MESSAGE( !combinedFilter->permitsSource(e0), "Source vertex should not be permitted");
    }

    {
        filters::EdgeRegexFilter edgeFilter(".*");
        filters::VertexRegexFilter nodeFilter(".*");

        Filter< Edge::Ptr >::Ptr filter(new filters::CombinedEdgeRegexFilter(nodeFilter, edgeFilter, nodeFilter));
        BOOST_REQUIRE_MESSAGE( filter->permits(e0), "Edge filter should permit");

        EdgeContextFilter::Ptr combinedFilter = boost::dynamic_pointer_cast<filters::CombinedEdgeRegexFilter>(filter);
        BOOST_REQUIRE_MESSAGE( combinedFilter->permitsTarget(e0), "Target vertex should be permitted");
        BOOST_REQUIRE_MESSAGE( combinedFilter->permitsSource(e0), "Source vertex should be permitted");
    }
}

BOOST_AUTO_TEST_SUITE_END()
