#include "GexfReader.hpp"
#include <base-logging/Logging.hpp>
#include <libgexf/nodeiter.h>
#include <libgexf/edgeiter.h>
#include <libgexf/filereader.h>
#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

namespace graph_analysis {
namespace io {

void GexfReader::read(const std::string& filename, BaseGraph::Ptr graph)
{
    // loading relevant gexf lib components
    libgexf::FileReader* reader = new libgexf::FileReader();
    reader->init(filename.c_str());
    reader->slurp(); // TODO: gexf.xsd schema needs to be downloaded and locally stored for a viable validation test with libgexf::SchemaValidator (see http://gexf.net/format/schema.html)

    // unwrapping the already parsed graph
    libgexf::GEXF gexf = reader->getGEXFCopy();
    libgexf::DirectedGraph& gexf_graph = gexf.getDirectedGraph();
    libgexf::Data& data = gexf.getData();

    std::string nodeClassAttr;
    std::string nodeLabelAttr;
    {
        libgexf::AttributeIter* gexf_attribute_it = data.getNodeAttributeColumn();
        while(gexf_attribute_it->hasNext())
        {
            libgexf::t_id id = gexf_attribute_it->next();
            std::string attributeLabel = gexf_attribute_it->currentTitle();

            if(attributeLabel == "class" || attributeLabel == "CLASS")
            {
                nodeClassAttr = id;
            }
            if(attributeLabel == "label" || attributeLabel == "LABEL")
            {
                nodeLabelAttr = id;
            }
        }

        if(nodeClassAttr.empty())
        {
            LOG_WARN_S << "No node attribute 'class' found";
        }
        if(nodeLabelAttr.empty())
        {
            LOG_WARN_S << "No node attribute 'label' found";
        }
    }

    std::string edgeClassAttr;
    std::string edgeLabelAttr;
    {

        libgexf::AttributeIter* gexf_attribute_it = data.getEdgeAttributeColumn();
        while(gexf_attribute_it->hasNext())
        {
            libgexf::t_id id = gexf_attribute_it->next();
            std::string attributeLabel = gexf_attribute_it->currentTitle();

            if(attributeLabel == "class" || attributeLabel == "CLASS")
            {
                edgeClassAttr = id;
            }
            if(attributeLabel == "label" || attributeLabel == "LABEL")
            {
                edgeLabelAttr = id;
            }
        }

        if(edgeClassAttr.empty())
        {
            LOG_WARN_S << "No edge attribute 'class' found";
        }
        if(edgeLabelAttr.empty())
        {
            LOG_WARN_S << "No edge attribute 'label' found";
        }
    }

    // storing the graph elements to the internal baseGraph
    graph->clear();
    VertexMap vertexMap;
    // storing the nodes and their properties
    libgexf::NodeIter* node_it = gexf_graph.getNodes();
    // having this pointer might come handy...
    VertexTypeManager *vManager = VertexTypeManager::getInstance();
    while(node_it->hasNext())
    {
        libgexf::t_id current = node_it->next();
        std::string nodeLabel = "";
        if(!nodeLabelAttr.empty())
        {
            nodeLabel = data.getNodeAttribute(current, nodeLabelAttr);
        } else if(data.hasNodeLabel(current))
        {
            nodeLabel = data.getNodeLabel(current);
        }

        std::string nodeClass = vManager->getDefaultType();
        if(!nodeClassAttr.empty())
        {
            nodeClass = data.getNodeAttribute(current, nodeClassAttr);
        }

        Vertex::Ptr vertex;
        try {
            vertex = vManager->createVertex(nodeClass, nodeLabel, true);
        } catch(const std::exception& e)
        {
            LOG_WARN_S << "Unsupported vertex type: '" << nodeClass << "' -- will use a placeholder node: " << e.what();
            vertex = Vertex::Ptr(new Vertex("Instance of unsupported vertex type: " + nodeClass + ": " + nodeLabel));
        }
        graph->addVertex(vertex);

        std::vector<std::string> attributes = vManager->getAttributes(vertex->getClassName());

        uint32_t memberCount = 0;
        std::vector<std::string>::const_iterator attributesIt = attributes.begin();
        for(; attributesIt != attributes.end(); ++attributesIt)
        {
            std::stringstream attrId;
            attrId << vertex->getClassName() << "-attribute-" << memberCount++;
            /// Retrieve GEXF data
            std::string attributeData = data.getNodeAttribute(current, attrId.str());

            io::AttributeSerializationCallbacks callbacks = vManager->getAttributeSerializationCallbacks(vertex->getClassName(),*attributesIt);
            (vertex.get()->*callbacks.deserializeFunction)(attributeData);
        }

        vertexMap[current] = vertex;
    }
    // storing the edges and their properties
    libgexf::EdgeIter* edge_it = gexf_graph.getEdges();
    // having this pointer might come handy...
    EdgeTypeManager *eManager = EdgeTypeManager::getInstance();
    while(edge_it->hasNext())
    {
        libgexf::t_id current = edge_it->next();
        std::string edgeLabel = "";
        if(!edgeLabelAttr.empty())
        {
            edgeLabel = data.getEdgeAttribute(current, edgeLabelAttr);
        } else if(data.hasEdgeLabel(current))
        {
            edgeLabel = data.getEdgeLabel(current);
        }

        std::string edgeClass = eManager->getDefaultType();
        if(!edgeClassAttr.empty())
        {
            edgeClass = data.getEdgeAttribute(current, edgeClassAttr);
        }

        Vertex::Ptr sourceVertex = vertexMap[edge_it->currentSource()]; // NOTE: assumes the .gexf(.xml) file is valid
        Vertex::Ptr targetVertex = vertexMap[edge_it->currentTarget()]; // NOTE: assumes the .gexf(.xml) file is valid
        Edge::Ptr edge;
        try {
            edge = eManager->createEdge(edgeClass, sourceVertex, targetVertex, edgeLabel, true);
        } catch(const std::exception& e)
        {
            LOG_WARN_S << "Unsupported edge type: '" << edgeClass << "' -- will use a placeholder edge: " << e.what();

            std::stringstream ss;
            std::set<std::string> supportedTypes = eManager->getSupportedTypes();
            std::set<std::string>::const_iterator cit = supportedTypes.begin();
            for(; cit != supportedTypes.end(); ++cit)
            {
                ss << *cit << ",";
            }
            LOG_WARN_S << "Supported types are: " << ss.str();
            edge = Edge::Ptr(new Edge(sourceVertex, targetVertex, "Instance of unsupported edge type: " + edgeClass + ": " + edgeLabel));
        }
        graph->addEdge(edge);


        std::vector<std::string> attributes = eManager->getAttributes(edge->getClassName());

        uint32_t memberCount = 0;
        std::vector<std::string>::const_iterator attributesIt = attributes.begin();
        for(; attributesIt != attributes.end(); ++attributesIt)
        {
            std::stringstream attrId;
            attrId << edge->getClassName() << "-attribute-" << memberCount++;
            /// Retrieve GEXF data
            std::string attributeData = data.getEdgeAttribute(current, attrId.str());

            if(!attributeData.empty())
            {
                io::AttributeSerializationCallbacks callbacks = eManager->getAttributeSerializationCallbacks(edge->getClassName(),*attributesIt);
                (edge.get()->*callbacks.deserializeFunction)(attributeData);
            }
        }
    }

    delete reader;
}

} // end namespace io
} // end namespace graph_analysis
