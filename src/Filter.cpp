#include "Filter.hpp"
#include "MapInitializer.hpp"

namespace graph_analysis {
namespace filters {

std::map<Type, std::string> TypeTxt = InitMap<Type,std::string>
    (CONTENT, "CONTENT")
    (CLASS, "CLASS");

std::map<std::string, Type> TxtType = InitMap<std::string, Type>
    ("CONTENT", CONTENT)
    ("CLASS", CLASS);

} // end namespace filters
} // end namespace graph_analysis
