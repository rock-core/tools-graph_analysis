#ifndef GRAPH_ANALYSIS_UTILS_FILESYSTEM_HPP
#define GRAPH_ANALYSIS_UTILS_FILESYSTEM_HPP

#include <cstdio>
#include <string>
#include <stdexcept>

namespace graph_analysis {
namespace utils {

class Filesystem
{
public:
    static void copy(const std::string& from,
            const std::string& to);
};

} // end namespace utils
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_UTILS_FILESYSTEM_HPP
