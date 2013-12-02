#include <boost/test/unit_test.hpp>
#include <graph/Dummy.hpp>

using namespace graph;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    graph::DummyClass dummy;
    dummy.welcome();
}
