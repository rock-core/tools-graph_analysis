#include "VertexTypeManager.hpp"

#include <boost/assign/list_of.hpp>
#include <base-logging/Logging.hpp>

namespace graph_analysis {

VertexTypeManager::VertexTypeManager()
    : base::Singleton<VertexTypeManager>()
    , AttributeManager()
{
    Vertex::Ptr vertex(new Vertex());
    registerType(vertex);
    setDefaultType( vertex->getClassName() );

}

void VertexTypeManager::registerType(const Vertex::Ptr& vertex, bool throwOnAlreadyRegistered)
{
    //Create a empty structure for this type to make sure getAttributes raise if a unregistered vertex type is queried
    registerType(vertex->getClassName(), vertex, throwOnAlreadyRegistered);
}

void VertexTypeManager::registerType(const vertex::Type& type, const Vertex::Ptr& node, bool throwOnAlreadyRegistered)
{
    assert(node);

    if(node->getClassName() != type)
    {
        throw std::runtime_error("graph_analysis::VertexTypeManager: cannot"
            "register vertex of type " + type + " it seems the getClassName()"
            "function of this class is implemented wrong it returned '" +
            node->getClassName() + "'");
    }

    try {
        vertexByType(type, true);
        LOG_INFO_S << "VertexType '" + type + "' is already registered.";
    } catch(...)
    {
        LOG_INFO_S << "VertexType '" + type + "' is newly registered";
        mTypeMap[type] = node;
        mRegisteredTypes.insert(type);

        activateAttributedType(type);
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
        LOG_DEBUG_S << "Using default VertexType '" << mDefaultVertexType << "'";
        return mTypeMap[mDefaultVertexType];
    }

    LOG_DEBUG_S << "VertexType '" + type + "' found.";
    return it->second;
}

void VertexTypeManager::setDefaultType(const std::string& defaultType)
{
    if( std::find(mRegisteredTypes.begin(), mRegisteredTypes.end(), defaultType) == mRegisteredTypes.end())
    {
        throw std::runtime_error("graph_analysis::VertexTypeManager::setDefaultVertexType: type '"
                + defaultType + "' is not a registered vertex type");
    }
    mDefaultVertexType = defaultType;
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

} // end namespace graph_analysis
