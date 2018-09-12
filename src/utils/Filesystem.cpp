#include "Filesystem.hpp"

namespace graph_analysis {
namespace utils {

std::string Filesystem::validateProgramAvailability(const std::string& programName)
{
    if(programExists(programName))
    {
        return programName;
    }
    throw std::invalid_argument("graph_analysis::utils::Filesystem::validateAvailability: "
            "could not find path to executable: '" + programName + "'");
}

bool Filesystem::programExists(const std::string& programName)
{
    std::string cmd = "which " + programName + " > /dev/null";
    if(0 == system(cmd.c_str()))
    {
        return true;
    } else {
        return false;
    }
}

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
