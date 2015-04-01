#include <boost/test/unit_test.hpp>
#include <graph_analysis/percolation/strategies/RandomDraw.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(percolation_test_suite)

BOOST_AUTO_TEST_CASE(random_draw)
{
    using namespace graph_analysis::percolation::strategies;

    {
        RandomDraw randomDraw(1);
        BOOST_REQUIRE_THROW(randomDraw.drawItem(), std::runtime_error);

        randomDraw.setProbabilityDensity(0, 0.0);
        BOOST_REQUIRE_THROW(randomDraw.drawItem(), std::runtime_error);

        randomDraw.setProbabilityDensity(1, 2.0);
        GraphElementId id = randomDraw.drawItem();
        BOOST_REQUIRE_MESSAGE(id == 1, "Draw item with id 1, but got " << id);
        BOOST_REQUIRE_NO_THROW(randomDraw.removeItem(id));
        BOOST_REQUIRE_THROW(randomDraw.drawItem(), std::runtime_error);
    }

    {
        RandomDraw randomDraw;
        BOOST_REQUIRE_THROW(randomDraw.drawItem(), std::runtime_error);

        randomDraw.setProbabilityDensity(0, 0.0);
        randomDraw.setProbabilityDensity(1, 1.0);
        randomDraw.setProbabilityDensity(2, 10.0);
        std::map<int, int> drawCount;
        for(int i = 0; i < 100000; ++i)
        {
            GraphElementId id = randomDraw.drawItem();
            drawCount[id]++;
        }

        BOOST_REQUIRE_MESSAGE(drawCount[0] == 0, "Zero probability never drawn: " << drawCount[0]);
        BOOST_REQUIRE_MESSAGE(drawCount[1] < drawCount[2], "1 less drawn than 2");
        double factor = drawCount[2]/drawCount[1];
        BOOST_REQUIRE_MESSAGE( factor > 8, "2 more often draw than 1 : factor " << factor);
    }
}

BOOST_AUTO_TEST_SUITE_END()
