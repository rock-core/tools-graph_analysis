#include "GraphMLReader.hpp"
#include <base-logging/Logging.hpp>
#include "../VertexTypeManager.hpp"
#include "../EdgeTypeManager.hpp"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

namespace graph_analysis {
namespace io {

GraphMLReader::GraphMLNode GraphMLReader::getNode(xmlNodePtr node, xmlXPathContextPtr context) const
{
    xmlNodePtr originalContextNode = context->node;
    xmlXPathSetContextNode(node, context);

    GraphMLNode graphmlNode;
    {
        xmlChar* property = xmlGetProp(node, (xmlChar*) "id");
        graphmlNode.id = std::string( (const char*) property);
    }

    VertexTypeManager *vManager = VertexTypeManager::getInstance();
    std::string nodeClass = vManager->getDefaultType();
    std::string label;
    std::string uuid;
    {
        xmlChar* xpath = (xmlChar*) "g:data[attribute::key=\"0\"]";
        xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
        if(result)
        {
            xmlChar* node_txt = xmlXPathCastNodeSetToString(result->nodesetval);
            nodeClass = (const char*) node_txt;
            xmlXPathFreeObject(result);
        }
    }
    {
        xmlChar* xpath = (xmlChar*) "g:data[attribute::key=\"1\"]";
        xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
        if(result)
        {
            xmlChar* node_txt = xmlXPathCastNodeSetToString(result->nodesetval);
            label = (const char*) node_txt;
            xmlXPathFreeObject(result);
        }
    }
    {
        xmlChar* xpath = (xmlChar*) "g:data[attribute::key=\"2\"]";
        xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
        if(result)
        {
            xmlChar* node_txt = xmlXPathCastNodeSetToString(result->nodesetval);
            uuid = (const char*) node_txt;
            xmlXPathFreeObject(result);
        }
    }
    Vertex::Ptr vertex = vManager->createVertex(nodeClass, label, true);
    if(!uuid.empty())
    {
        setUuid(vertex, uuid);
    }

    // Handle the local attributes
    std::vector<std::string> attributes = vManager->getAttributes(nodeClass);
    uint32_t memberCount = 0;
    std::vector<std::string>::const_iterator attributesIt = attributes.begin();
    for(; attributesIt != attributes.end(); ++attributesIt)
    {
        std::stringstream attrId;
        attrId << nodeClass << "-attribute-" << memberCount++;

        std::string attributeData;
        std::string query = "g:data[attribute::key=\"" + attrId.str() + "\"]";
        xmlChar* xpath = (xmlChar*) query.c_str();
        xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
        if(result)
        {
            xmlChar* node_txt = xmlXPathCastNodeSetToString(result->nodesetval);
            attributeData = (const char*) node_txt;
            xmlXPathFreeObject(result);
        }

        if(!attributeData.empty())
        {
            io::AttributeSerializationCallbacks callbacks = vManager->getAttributeSerializationCallbacks(vertex->getClassName(),*attributesIt);
            // deserialize data into the vertex
            (vertex.get()->*callbacks.deserializeFunction)(attributeData);
        }
    }

    graphmlNode.vertex = vertex;

    xmlXPathSetContextNode(originalContextNode, context);
    return graphmlNode;
}

GraphMLReader::GraphMLEdge GraphMLReader::getEdge(xmlNodePtr node, xmlXPathContextPtr context) const
{
    xmlNodePtr originalContextNode = context->node;
    xmlXPathSetContextNode(node, context);

    GraphMLEdge graphmlEdge;
    {
        xmlChar* property = xmlGetProp(node, (xmlChar*) "id");
        graphmlEdge.id = std::string( (const char*) property);

        xmlChar* source = xmlGetProp(node, (xmlChar*) "source");
        graphmlEdge.source = std::string( (const char*) source);

        xmlChar* target = xmlGetProp(node, (xmlChar*) "target");
        graphmlEdge.target = std::string( (const char*) target);
    }

    EdgeTypeManager *eManager = EdgeTypeManager::getInstance();
    std::string edgeClass = eManager->getDefaultType();
    std::string label;
    std::string uuid;
    {
        xmlChar* xpath = (xmlChar*) "g:data[attribute::key=\"0\"]";
        xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
        if(result)
        {
            xmlChar* node_txt = xmlXPathCastNodeSetToString(result->nodesetval);
            edgeClass = (const char*) node_txt;
            xmlXPathFreeObject(result);
        }
    }
    {
        xmlChar* xpath = (xmlChar*) "g:data[attribute::key=\"1\"]";
        xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
        if(result)
        {
            xmlChar* node_txt = xmlXPathCastNodeSetToString(result->nodesetval);
            label = (const char*) node_txt;
            xmlXPathFreeObject(result);
        }
    }
    {
        xmlChar* xpath = (xmlChar*) "g:data[attribute::key=\"2\"]";
        xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
        if(result)
        {
            xmlChar* node_txt = xmlXPathCastNodeSetToString(result->nodesetval);
            uuid = (const char*) node_txt;
            xmlXPathFreeObject(result);
        }
    }
    Edge::Ptr edge = eManager->createEdge(edgeClass, label, true);
    if(!uuid.empty())
    {
        setUuid(edge, uuid);
    }


    std::vector<std::string> attributes = eManager->getAttributes(edge->getClassName());
    uint32_t memberCount = 0;
    std::vector<std::string>::const_iterator attributesIt = attributes.begin();
    for(; attributesIt != attributes.end(); ++attributesIt)
    {
        std::stringstream attrId;
        attrId << edge->getClassName() << "-attribute-" << memberCount++;

        std::string attributeData;
        std::string query = "g:data[attribute::key=\"" + attrId.str() + "\"]";
        xmlChar* xpath = (xmlChar*) query.c_str();
        xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
        if(result)
        {
            xmlChar* node_txt = xmlXPathCastNodeSetToString(result->nodesetval);
            attributeData = (const char*) node_txt;
            xmlXPathFreeObject(result);
        }

        if(!attributeData.empty())
        {
            io::AttributeSerializationCallbacks callbacks = eManager->getAttributeSerializationCallbacks(edge->getClassName(),*attributesIt);
            (edge.get()->*callbacks.deserializeFunction)(attributeData);
        }
    }

    graphmlEdge.edge = edge;
    xmlXPathSetContextNode(originalContextNode, context);

    return graphmlEdge;
}

void GraphMLReader::read(const std::string& url, BaseGraph::Ptr graph)
{
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    // The resulting document tree
    xmlDocPtr doc = NULL;
    //xmlParserOption options =  XML_PARSE_NOENT; // http://xmlsoft.org/html/libxml-parser.html#xmlParserOption

    try {
        // Ignore spaces between nodes (otherwise many text nodes have to be
        // considered which are effectively just empty space due to formatting)
        xmlKeepBlanksDefault(0);
        doc = xmlParseFile(url.c_str());
        if(doc == NULL)
        {
            throw std::runtime_error("templ::io::MissionReader::fromFile: Failed to parse url '" + url + "'");
        }

        //http://xmlsoft.org/tutorial/ar01s05.html
        //https://www.w3.org/TR/1999/REC-xpath-19991116/
        xmlXPathContextPtr context = xmlXPathNewContext(doc);
        xmlXPathRegisterNs(context,  BAD_CAST "g", BAD_CAST "http://graphml.graphdrawing.org/xmlns");
        {
            VertexMap vertices;
            {
                xmlChar* xpath = (xmlChar*) "/g:graphml/g:graph[1]/g:node";
                xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
                if(result)
                {
                    xmlNodeSetPtr nodeset = result->nodesetval;
                    for(int i = 0; i < nodeset->nodeNr; ++i)
                    {
                        try {
                            GraphMLNode graphmlNode= getNode(nodeset->nodeTab[i],context);
                            vertices[graphmlNode.id] = graphmlNode.vertex;

                            graph->addVertex(graphmlNode.vertex);
                        } catch(const std::exception& e)
                        {
                            LOG_WARN_S << "Failed to extract node from graphml: " << e.what();
                        }
                    }
                }
            }
            {
                xmlChar* xpath = (xmlChar*) "/g:graphml/g:graph[1]/g:edge";
                xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
                if(result)
                {
                    xmlNodeSetPtr nodeset = result->nodesetval;
                    for(int i = 0; i < nodeset->nodeNr; ++i)
                    {
                        try {
                            GraphMLEdge graphmlEdge = getEdge(nodeset->nodeTab[i],context);
                            Vertex::Ptr sourceVertex = vertices.at(graphmlEdge.source);
                            Vertex::Ptr targetVertex = vertices.at(graphmlEdge.target);
                            graphmlEdge.edge->setSourceVertex(sourceVertex);
                            graphmlEdge.edge->setTargetVertex(targetVertex);

                            graph->addEdge(graphmlEdge.edge);
                        } catch(const std::exception& e)
                        {
                            LOG_WARN_S << "Failed to extract edge from graphml: " << e.what();
                        }
                    }
                }
            }
        }
    } catch(const std::exception& e)
    {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        throw;
    }

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
}

} // end namespace io
} // end namespace graph_analysis
