#include <boost/test/unit_test.hpp>
#include <graph_analysis/GraphElement.hpp>
#include <graph_analysis/Vertex.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(graph_element)

BOOST_AUTO_TEST_CASE(registration)
{
    GraphElementUuid uuid0;
    {
        Vertex::Ptr v0 = make_shared<Vertex>("v0");
        Vertex::Ptr v1 = make_shared<Vertex>("v0");

        uuid0 = v0->getUuid();
        BOOST_TEST_MESSAGE("Uuid 0 " << uuid0);

        GraphElementUuid uuid1 = v1->getUuid();
        BOOST_TEST_MESSAGE("Uuid 1 " << uuid1);

        GraphElement::Ptr r0 = GraphElement::fromUuid(uuid0);
        BOOST_REQUIRE_MESSAGE(r0->getLabel() == v0->getLabel(), "V0 and "
                " retrieved via uuid are the same");

        GraphElement::Ptr r1 = GraphElement::fromUuid(uuid1);
        BOOST_REQUIRE_MESSAGE(r1->getLabel() == v1->getLabel(), "V1 and "
                " retrieved via uuid are the same");
    }

    BOOST_REQUIRE_THROW(GraphElement::fromUuid(uuid0), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
