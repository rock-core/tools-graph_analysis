#include "Attribute.hpp"
#include <sstream>

namespace graph_analysis {

Attribute::Attribute()
{}

Attribute::Attribute(const std::string& id,
         const std::string memberName,
         const std::string className,
         const std::string& type)
     : mId(id)
     , mMemberName(memberName)
     , mClassName(className)
     , mType(type)
 {}

std::string Attribute::toString() const
{
    std::stringstream ss;
    ss << "id: " << mId
        << ", member name: " << mMemberName
        << ", class name: " << mClassName
        << ", type: " << mType;
    return ss.str();
}

} // end namespace graph_analysis
