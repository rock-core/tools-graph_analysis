#include <boost/test/unit_test.hpp>
#include <omviz/Dummy.hpp>

using namespace omviz;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    omviz::DummyClass dummy;
    dummy.welcome();
}
