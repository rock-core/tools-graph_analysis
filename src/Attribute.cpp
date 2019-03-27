#include "Attribute.hpp"
#include <sstream>

namespace graph_analysis {

Attribute::Attribute()
{}

Attribute::Attribute(const std::string& id,
         const std::string name,
         const std::string& type)
     : mId(id)
     , mName(name)
     , mType(type)
 {}

std::string Attribute::toString() const
{
    std::stringstream ss;
    ss << "id: " << mId << ", name: " << mName << ", type: " << mType;
    return ss.str();
}

} // end namespace graph_analysis
