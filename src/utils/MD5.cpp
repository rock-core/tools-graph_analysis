#include "MD5.hpp"
#include <openssl/md5.h>
#include <sstream>
#include <fstream>
#include <iomanip>

namespace graph_analysis {
namespace utils {

std::string MD5Digest::md5Sum(const std::string& filename)
{
    MD5_CTX context;
    MD5_Init(&context);

    std::ifstream input(filename, std::ios::binary);
    char fileBuffer[4096];
    while(input.read(fileBuffer, sizeof(fileBuffer)) || input.gcount())
    {
        MD5_Update(&context, fileBuffer, input.gcount());
    }

    unsigned char digest[MD5_DIGEST_LENGTH] = {};
    MD5_Final(digest, &context);
    char outputBuffer[MD5_DIGEST_LENGTH*2 + 1];
    size_t i = 0;
    for(i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        sprintf(outputBuffer + (i*2), "%02x", digest[i]);
    }
    outputBuffer[i*2] = 0;
    return std::string(outputBuffer);
}

} // end namespace utils
} // end namespace graph_analysis
