#ifndef GRAPH_ANALYSIS_TEST_UTILS_TEST
#define GRAPH_ANALYSIS_TEST_UTILS_TEST

#include <boost/test/unit_test.hpp>
#include <graph_analysis/utils/MD5.hpp>
#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(utils)

BOOST_AUTO_TEST_CASE(md5sum)
{
    std::string filename = getRootDir() + "test/data/md5sum.data";
    std::string md5sum = graph_analysis::utils::MD5Digest::md5Sum(filename);

    BOOST_REQUIRE_MESSAGE(md5sum == "cb8ebbad3eb2b734928430a33311792f",
            "Md5Sum should be available was '" << md5sum << "'");
}

BOOST_AUTO_TEST_SUITE_END()
#endif

