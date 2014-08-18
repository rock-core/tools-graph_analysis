#include <boost/test/unit_test.hpp>
#include <graph_analysis/filters/RegexFilters.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_CASE(it_should_filter_with_regex)
{
    Vertex::Ptr v0( new Vertex());

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

}
