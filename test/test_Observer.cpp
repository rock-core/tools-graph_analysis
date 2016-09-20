#include <boost/test/unit_test.hpp>
#include <graph_analysis/TransactionObserver.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(observer)

BOOST_AUTO_TEST_CASE(transactionObserver)
{
    BaseGraphObserver::Ptr ptr( new BaseGraphObserver() );
    TransactionObserver::Ptr obs = TransactionObserver::getInstance( ptr );

    // TODO do some actual testing
}

BOOST_AUTO_TEST_SUITE_END()
