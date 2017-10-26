#ifndef GRAPH_ANALYSIS_UTILS_MD5_HPP
#define GRAPH_ANALYSIS_UTILS_MD5_HPP

#include <string>

namespace graph_analysis {
namespace utils {

class MD5Digest
{
    public:
        static std::string md5Sum(const std::string& filename);
};

} // end namespace utils
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_UTILS_MD5_HPP
