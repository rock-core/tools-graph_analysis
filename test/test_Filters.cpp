#include <boost/test/unit_test.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>
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
        BOOST_REQUIRE_MESSAGE(! filter->matches(v0), "Permit all filter matches no vertex");

        filters::VertexRegexFilter::Ptr contentFilter(new filters::VertexRegexFilter(".*graph_analysis::.*", filters::CONTENT, false));
        filter->add(contentFilter);

        BOOST_REQUIRE_MESSAGE(filter->matches(v0), "Content filter: '" << contentFilter->getName() << "' should filter: " << v0->toString());
    }

    {
        Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filters::PermitAll< Vertex::Ptr >() );
        BOOST_REQUIRE_MESSAGE(! filter->matches(v0), "Permit all filter matches no vertex");

        filters::VertexRegexFilter::Ptr contentFilter0(new filters::VertexRegexFilter(".*dummy.*", filters::CONTENT, false));
        filters::VertexRegexFilter::Ptr contentFilter1(new filters::VertexRegexFilter("graph_analysis.*", filters::CONTENT, false));
        filter->add(contentFilter0);
        filter->add(contentFilter1);

        BOOST_REQUIRE_MESSAGE(filter->matches(v0), "Content filter: '" << contentFilter0->getName() << " or " << contentFilter1->getName() << "' should filter: " << v0->toString());
    }
    {
        filters::EdgeContextFilter::Ptr filter( new graph_analysis::filters::PermitAll< Edge::Ptr >());
        BOOST_REQUIRE_MESSAGE(! filter->matches(e0), "Permit all filter should match no edge");
        BOOST_REQUIRE_MESSAGE(! filter->matchesTarget(e0), "Permit all filter should not apply to any edge's target node");
        BOOST_REQUIRE_MESSAGE(! filter->matchesSource(e0), "Permit all filter should not apply to any edge's source node");
    }

    {
        filters::EdgeRegexFilter edgeFilter;
        filters::VertexRegexFilter nodeFilter;

        Filter< Edge::Ptr >::Ptr filter(new filters::CombinedEdgeRegexFilter(nodeFilter, edgeFilter, nodeFilter));
        BOOST_REQUIRE_MESSAGE( !filter->matches(e0), "Edge filter should not filter anything by default");

        filters::CombinedEdgeRegexFilter::Ptr combinedFilter = boost::dynamic_pointer_cast<filters::CombinedEdgeRegexFilter>(filter);
        BOOST_REQUIRE_MESSAGE( !combinedFilter->matchesTarget(e0), "CombinedEdgeRegexFilter should not filter target vertex by default");
        BOOST_REQUIRE_MESSAGE( !combinedFilter->matchesSource(e0), "CombinedEdgeRegexFilter should not filter source vertex by default");
    }

    {
        filters::EdgeRegexFilter edgeFilter(".*");
        filters::VertexRegexFilter nodeFilter(".*");

        Filter< Edge::Ptr >::Ptr filter(new filters::CombinedEdgeRegexFilter(nodeFilter, edgeFilter, nodeFilter));
        BOOST_REQUIRE_MESSAGE( filter->matches(e0), "Edge filter should filter all");

        filters::EdgeContextFilter::Ptr combinedFilter = boost::dynamic_pointer_cast<filters::CombinedEdgeRegexFilter>(filter);
        BOOST_REQUIRE_MESSAGE( combinedFilter->matchesTarget(e0), "Target vertex should be filtered");
        BOOST_REQUIRE_MESSAGE( combinedFilter->matchesSource(e0), "Source vertex should be filtered");
    }
}

BOOST_AUTO_TEST_SUITE_END()
