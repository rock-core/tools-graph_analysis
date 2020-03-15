#ifndef GRAPH_ANALYSIS_UTILS_FILESYSTEM_HPP
#define GRAPH_ANALYSIS_UTILS_FILESYSTEM_HPP

#include <string>

namespace graph_analysis {
namespace utils {

class Filesystem
{
public:

    /**
     * Validates if a program exists, and throws otherwise
     * \return programName, if path to programName has been found
     */
    static std::string validateProgramAvailability(const std::string& programName);

    /**
     * Test if a program exists using `which <program>`
     */
    static bool programExists(const std::string& programName);

    static void copy(const std::string& from,
            const std::string& to);
};

} // end namespace utils
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_UTILS_FILESYSTEM_HPP
