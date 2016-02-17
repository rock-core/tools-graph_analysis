#include <boost/test/unit_test.hpp>
#include <graph_analysis/Edge.hpp>
#include <graph_analysis/WeightedEdge.hpp>
#include <graph_analysis/NWeightedEdge.hpp>
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

    WeightedEdge::Ptr w0 = dynamic_pointer_cast<WeightedEdge>(edge0);
    WeightedEdge::Ptr w1 = dynamic_pointer_cast<WeightedEdge>(edge0_clone);

    BOOST_REQUIRE_MESSAGE(w0->getWeight() == w1->getWeight(), "Edge and clone have same weights after cloning");
    w1->setWeight(weight + 1.0);
    BOOST_REQUIRE_MESSAGE(w0->getWeight() != w1->getWeight(), "Edge and clone have different weights");
}

BOOST_AUTO_TEST_CASE(nweighted_edge)
{
    std::string name = "vertex-name";

    Vertex::Ptr vertex0( new Vertex(name));
    Vertex::Ptr vertex0_clone = vertex0->clone();

    BOOST_REQUIRE_MESSAGE(vertex0->getLabel() == vertex0_clone->getLabel(), "Vertex clone has same name after cloning");
    vertex0_clone->setLabel("otherLabel");
    BOOST_REQUIRE_MESSAGE(vertex0->getLabel() != vertex0_clone->getLabel(), "Vertex clone has different name");


    std::vector<double> weights(2);
    weights[0] = 1.0;
    weights[1] = 2.0;

    Edge::Ptr edge0(new NWeightedEdge<double>(weights) );
    Edge::Ptr edge0_clone = edge0->clone();

    NWeightedEdge<double>::Ptr w0 = dynamic_pointer_cast<NWeightedEdge<double> >(edge0);
    NWeightedEdge<double>::Ptr w1 = dynamic_pointer_cast<NWeightedEdge<double> >(edge0_clone);

    BOOST_REQUIRE_MESSAGE(w0->getWeights() == w1->getWeights(), "Edge and clone have same weights after cloning");

    w1->setWeight(w1->getWeight(0) + 1.0, 0);
    w1->setWeight(w1->getWeight(0) + 2.0, 1);
    BOOST_REQUIRE_MESSAGE(w0->getWeights() != w1->getWeights(), "Edge and clone have different weights");

    BOOST_REQUIRE_THROW(w0->getWeight(10), std::out_of_range);

    std::vector<double> weights3d(3);
    BOOST_REQUIRE_THROW( NWeightedEdge<double>::Ptr(new NWeightedEdge<double>(weights3d)), std::invalid_argument );
}

BOOST_AUTO_TEST_SUITE_END()

