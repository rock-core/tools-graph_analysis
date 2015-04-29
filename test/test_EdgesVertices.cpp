#include <boost/test/unit_test.hpp>
#include <graph_analysis/Edge.hpp>
#include <graph_analysis/WeightedEdge.hpp>
#include <graph_analysis/Vertex.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(edges_and_vertices)

BOOST_AUTO_TEST_CASE(clone)
{
    std::string name = "vertex-name";

    Vertex::Ptr vertex0( new Vertex(name));
    Vertex::Ptr vertex0_clone = vertex0->clone();

    BOOST_REQUIRE_MESSAGE(vertex0->getLabel() == vertex0_clone->getLabel(), "Vertex clone has same name after cloning");
    vertex0_clone->setLabel("otherLabel");
    BOOST_REQUIRE_MESSAGE(vertex0->getLabel() != vertex0_clone->getLabel(), "Vertex clone has different name");


    double weight = 10.0;
    Edge::Ptr edge0(new WeightedEdge(weight));
    Edge::Ptr edge0_clone = edge0->clone();
    
    WeightedEdge::Ptr w0 = boost::dynamic_pointer_cast<WeightedEdge>(edge0);
    WeightedEdge::Ptr w1 = boost::dynamic_pointer_cast<WeightedEdge>(edge0_clone);

    BOOST_REQUIRE_MESSAGE(w0->getWeight() == w1->getWeight(), "Edge and clone have same weights after cloning");
    w1->setWeight(weight + 1.0);
    BOOST_REQUIRE_MESSAGE(w0->getWeight() != w1->getWeight(), "Edge and clone have different weights");
}

BOOST_AUTO_TEST_SUITE_END()

