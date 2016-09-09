#include "VertexTypeManager.hpp"

#include <boost/assign/list_of.hpp>
#include <base-logging/Logging.hpp>

namespace graph_analysis {

VertexTypeManager::VertexTypeManager()
{
    registerType(Vertex::Ptr(new Vertex()));
}

void VertexTypeManager::registerType(const vertex::Type& type, Vertex::Ptr node, bool throwOnAlreadyRegistered)
{
    assert(node.get());

    if(node->getClassName() != type){
        throw std::runtime_error("Cannot register vertex of type " + type + " it seems the getClassName() funtion of this class is implemented wrong it returned " + node->getClassName());
    }
    try {
        vertexByType(type, true);
        LOG_INFO_S << "VertexType '" + type + "' is already registered.";
    } catch(...)
    {
        LOG_INFO_S << "VertexType '" + type + "' is newly registered";
        mTypeMap[type] = node;
        mRegisteredTypes.insert(type);
        mRegisteredCallbacks[type];
        return;
    }
    if(throwOnAlreadyRegistered)
    {
        throw std::runtime_error("graph_analysis::VertexTypeManager::registerType: type '" + type + "' is already registered");
    }
}

Vertex::Ptr VertexTypeManager::vertexByType(const vertex::Type& type, bool throwOnDefault)
{
    TypeMap::const_iterator it = mTypeMap.find(type);
    if(it == mTypeMap.end())
    {
        LOG_DEBUG_S << "type '" + type + "' is not registered.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' is not registered");
        }
        LOG_DEBUG_S << "Using default VertexType " << graph_analysis::Vertex::vertexType() << ".";
        return mTypeMap[graph_analysis::Vertex::vertexType()];
    }

    LOG_DEBUG_S << "VertexType '" + type + "' found.";
    return it->second;
}

Vertex::Ptr VertexTypeManager::createVertex(const vertex::Type& type, const std::string& label, bool throwOnMissing)
{
    Vertex::Ptr v = vertexByType(type, throwOnMissing);
    if(!v){
        throw std::invalid_argument("graph_analysis::VertexTypeManager: cannot get node for type: " + type + " and label " + label);
    }
    Vertex::Ptr clonedVertex = v->clone();
    if(v->getClassName() != clonedVertex->getClassName())
    {
        throw std::runtime_error("graph_analysis::VertexTypeManager: cannot create cloned vertex of type " + v->getClassName() + " it seems the 'virtual Vertex* getClone() const' funtion of this class is implemented wrong");
    }

    clonedVertex->setLabel(label);
    return clonedVertex;
}

std::set<std::string> VertexTypeManager::getSupportedTypes()
{
    return mRegisteredTypes;
}

void VertexTypeManager::registerAttribute(const std::string &typeName, const std::string &attributeName, serialize_func_t sF, deserialize_func_t dsF, print_func_t pF)
{
    if(mRegisteredCallbacks.find(typeName) == mRegisteredCallbacks.end())
    {
        throw std::invalid_argument("Cannot register attribute for unknown Type: " + typeName);
    }
    MemberCallbacks mc = {sF,dsF,pF};
    mRegisteredCallbacks[typeName][attributeName] = mc;
}

std::list<std::string> VertexTypeManager::getMembers(const std::string &typeName)
{
    std::list<std::string> res;
    if(mRegisteredCallbacks.find(typeName) == mRegisteredCallbacks.end())
    {
        //No registered callbacks
        return res;
    }

    for(std::map<std::string, MemberCallbacks>::const_iterator it = mRegisteredCallbacks[typeName].begin(); it != mRegisteredCallbacks[typeName].end(); ++it)
    {
        res.push_back(it->first);
    }
    return res;
}

VertexTypeManager::MemberCallbacks VertexTypeManager::getMemberCallbacks(const std::string &typeName, const std::string &memberName)
{
    if(mRegisteredCallbacks.find(typeName) == mRegisteredCallbacks.end())
    {
        throw std::invalid_argument("Cannot get Callbacks for unknown type " + typeName);
    }
    if(mRegisteredCallbacks[typeName].find(memberName) == mRegisteredCallbacks[typeName].end())
    {
        throw std::invalid_argument("Cannot get Callbacks for unknown member" + memberName + " of " + typeName);
    }
    return mRegisteredCallbacks[typeName][memberName];
}

void VertexTypeManager::registerType(Vertex::Ptr vertex, bool throwOnAlreadyRegistered)
{
    //Create a empty structure for this type to make sure getMembers raise if a unregistered vertex type is queried
    registerType(vertex->getClassName(), vertex, throwOnAlreadyRegistered);
}

} // end namespace graph_analysis
