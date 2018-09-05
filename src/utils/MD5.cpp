#include "MD5.hpp"
#include <cstdio>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>

namespace graph_analysis {
namespace utils {

std::string MD5Digest::md5Sum(const std::string& filename)
{
    char fileBuffer[4096];
    std::string result;
    // use printf to not print a newline
    std::string cmd = "/usr/bin/md5sum " + filename + "| awk '{ printf $1 }'";
    FILE* pipe = popen(cmd.c_str(), "r");
    if(!pipe)
    {
        throw std::runtime_error("graph_analysis::utils::MD5Digest::md5sum:"
                " failed to retrieve md5sum for " + filename);
    }
    while(!feof(pipe))
    {
        if(fgets(fileBuffer, 4096, pipe) != NULL)
        {
            result += fileBuffer;
        }
    }
    pclose(pipe);
    return fileBuffer;
}

} // end namespace utils
} // end namespace graph_analysis
