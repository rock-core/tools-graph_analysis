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
        std::stringstream ss;
        ss << "graph_analysis::VertexTypeManager: cannot create cloned vertex of type " + v->getClassName() + " it seems the 'virtual Vertex* getClone() const' function of this class is implemented wrong";
        LOG_WARN_S << ss.str();
        throw std::runtime_error(ss.str());
    }

    clonedVertex->setLabel(label);
    return clonedVertex;
}

std::set<std::string> VertexTypeManager::getSupportedTypes() const
{
    return mRegisteredTypes;
}

std::vector<Attribute> VertexTypeManager::getKnownAttributes(const std::set<std::string>& classnames) const
{
    std::vector<Attribute> allAttributes;

    std::set<std::string> types = getSupportedTypes();
    for(std::set<std::string>::const_iterator type_it = types.begin(); type_it != types.end(); type_it++)
    {
        std::string classname = *type_it;
        if(!classnames.empty())
        {
            if( classnames.count(classname) == 0)
            {
                continue;
            }
        }
        uint32_t memberCount = 0;
        std::vector<std::string> attributes = AttributeManager::getAttributes(classname);
        std::vector<std::string>::const_iterator attributesIt = attributes.begin();
        for(; attributesIt != attributes.end(); attributesIt++)
        {
            std::stringstream attrId;
            attrId << *type_it << "-attribute-" << memberCount++;
            LOG_DEBUG_S << "Adding custom node attribute: id: " << attrId.str() << ", title: " << *attributesIt << ", type: STRING";
            allAttributes.push_back(Attribute(attrId.str(), *attributesIt, "STRING"));
        }
    }
    return allAttributes;
}

std::vector< std::pair<Attribute::Id, std::string> > VertexTypeManager::getAttributeValues(const Vertex::Ptr& vertex) const
{
    std::vector< std::pair<Attribute::Id, std::string> > attributeAssignments;

    std::vector<std::string> attributes = AttributeManager::getAttributes(vertex->getClassName());
    uint32_t memberCount = 0;
    std::vector<std::string>::const_iterator attributesIt = attributes.begin();
    for(; attributesIt != attributes.end(); ++attributesIt)
    {
        std::stringstream attrId;
        attrId << vertex->getClassName() << "-attribute-" << memberCount++;
        io::AttributeSerializationCallbacks callbacks = getAttributeSerializationCallbacks(vertex->getClassName(),*attributesIt);
        std::string data = (vertex.get()->*callbacks.serializeFunction)();

        if(!data.empty())
        {
            attributeAssignments.push_back(std::pair<Attribute::Id,std::string>(attrId.str(), data));
        }
    }
    return attributeAssignments;
}

} // end namespace graph_analysis
