#include "Filter.hpp"
#include <boost/assign/list_of.hpp>

namespace graph_analysis {
namespace filters {

std::map<Type, std::string> TypeTxt = boost::assign::map_list_of
    (CONTENT, "CONTENT")
    (CLASS, "CLASS");

std::map<std::string, Type> TxtType = boost::assign::map_list_of
    ("CONTENT", CONTENT)
    ("CLASS", CLASS);

} // end namespace filters
} // end namespace graph_analysis
