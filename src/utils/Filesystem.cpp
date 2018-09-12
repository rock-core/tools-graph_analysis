#include "Filesystem.hpp"

namespace graph_analysis {
namespace utils {

void Filesystem::copy(const std::string& from,
            const std::string& to)
{
    if(from == to)
    {
        return;
    }

    std::string cmd = "cp " + from + " " + to;
    if(0 != system(cmd.c_str()) )
    {
        throw std::runtime_error("graph_analysis::utils::Filesystem::copy:"
                " copying with command: '" + cmd + "' failed");
    }
}

} // end namespace utils
} // end namespace graph_analysis
