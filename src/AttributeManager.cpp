#include "AttributeManager.hpp"

namespace graph_analysis {

void AttributeManager::registerAttribute(const std::string& typeName, const std::string& attributeName,
        io::AttributeSerializationCallbacks::serialize_func_t sF,
        io::AttributeSerializationCallbacks::deserialize_func_t dsF,
        io::AttributeSerializationCallbacks::print_func_t pF)
{
    if(mRegisteredCallbacks.find(typeName) == mRegisteredCallbacks.end())
    {
        throw std::invalid_argument("graph_analysis::AttributeManager::registerAttribute: cannot register attribute for unknown type: " + typeName);
    }
    io::AttributeSerializationCallbacks mc = {sF,dsF,pF};
    mRegisteredCallbacks[typeName][attributeName] = mc;
}

std::vector<std::string> AttributeManager::getAttributes(const std::string& vertexTypeName) const
{
    std::vector<std::string> attributes;
    std::map<std::string, AttributeSerializationCallbackMap>::const_iterator cit = mRegisteredCallbacks.find(vertexTypeName);
    if(cit == mRegisteredCallbacks.end())
    {
        //No registered callbacks
        return attributes;
    } else {
        const AttributeSerializationCallbackMap& callbacks = cit->second;
        AttributeSerializationCallbackMap::const_iterator rit = callbacks.begin();
        for(; rit != callbacks.end(); ++rit)
        {
            attributes.push_back(rit->first);
        }
    }
    return attributes;
}

io::AttributeSerializationCallbacks AttributeManager::getAttributeSerializationCallbacks(const std::string& typeName, const std::string& memberName)
{
    if(mRegisteredCallbacks.find(typeName) == mRegisteredCallbacks.end())
    {
        throw std::invalid_argument("graph_analysis::AttributeManager: cannot get callbacks for unknown type '" + typeName + "'");
    }
    if(mRegisteredCallbacks[typeName].find(memberName) == mRegisteredCallbacks[typeName].end())
    {
        throw std::invalid_argument("graph_analysis::AttributeManager cannot get callbacks for unknown member '" + memberName + "' of type '" + typeName + "'");
    }
    return mRegisteredCallbacks[typeName][memberName];
}

} // end namespace graph_analysis
