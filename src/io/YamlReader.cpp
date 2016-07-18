#include "YamlReader.hpp"
#include <base-logging/Logging.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

namespace graph_analysis {
namespace io {

void YamlReader::read(const std::string& filename, BaseGraph::Ptr graph)
{
    const char *fname = filename.c_str();
    std::ifstream fin(fname);
    if(!fin.good())
    {
        // error checking
        LOG_ERROR_S << "failed to open input file '" << filename
                    << "' for graph import - no such file";
        return;
    }
    LOG_INFO_S << "importing graph from file '" << fname << "'";

    graph->clear();
    VertexMap vMap;
    std::string line;
    std::getline(fin, line);
    std::stringstream nodeStream, edgeStream;
    // tokenizing the nodes list string and the edges list string respectively
    while(!fin.eof())
    {
        if(std::string::npos != line.find("nodes:"))
        {
            std::getline(fin, line);
            while(std::string::npos == line.find("edges:"))
            {
                nodeStream << line << std::endl;
                if(!fin.eof())
                {
                    std::getline(fin, line);
                }
                else
                {
                    break;
                }
            }
            while(!fin.eof())
            {
                std::getline(fin, line);
                edgeStream << line << std::endl;
            }
        }
        else
        {
            std::getline(fin, line);
        }
    }
    // parsing nodes and then parsing edges from the respectively tokenized information
    parseNodes(nodeStream, graph, vMap);
    parseEdges(edgeStream, graph, vMap);
}


void YamlReader::parseNodes(std::stringstream& nodeStream, const BaseGraph::Ptr& graph, VertexMap& vMap) const
{
    char c;
    std::string idWord, labelWord, typeWord;
    while(!nodeStream.eof())
    {
        // looking for initial '-' character, preceding an individual node
        do
        {
            nodeStream >> c;
        }
        while('-' != c && !nodeStream.eof());
        if(nodeStream.eof())
        {
            break;
        }

        // parsing one node (one by one, all properties)
        idWord = nextToken("id:", nodeStream);
        typeWord = nextToken("type:", nodeStream);
        labelWord = nextToken("label:", nodeStream);
        // storing results
        Vertex::Ptr vertex = VertexTypeManager::getInstance()->createVertex(typeWord, labelWord);
        vMap[idWord] = vertex;
        graph->addVertex(vertex);
    }
}

void YamlReader::parseEdges(std::stringstream& edgeStream, const BaseGraph::Ptr& graph, VertexMap& vMap) const
{
    char c;
    std::string fromNodeWord, toNodeWord, labelWord;
    while(!edgeStream.eof())
    {
        // looking for initial '-' character, preceding an individual edge
        do
        {
            edgeStream >> c;
        }
        while('-' != c && !edgeStream.eof());
        if(edgeStream.eof())
        {
            break;
        }

        // parsing one edge (one by one, all properties)
        fromNodeWord = nextToken("fromNodeId:", edgeStream);
        toNodeWord = nextToken("toNodeId:", edgeStream);
        labelWord = nextToken("label:", edgeStream);
        // storing results
        Vertex::Ptr sourceVertex = vMap[fromNodeWord]; // NOTE: assumes the .yml (.yaml) file is valid
        Vertex::Ptr targetvertex = vMap[toNodeWord]; // NOTE: assumes the .yml (.yaml) file is valid
        Edge::Ptr edge(new Edge(sourceVertex, targetvertex, labelWord));
        graph->addEdge(edge);
    }
}

std::string YamlReader::nextToken(const std::string& keyword, std::stringstream& stream) const
{
    if(stream.eof())
    {
        die("Parsing error: keyword '" + keyword + "' failed to be found; End-Of-File was reached prematurely");
    }

    std::string word;
    stream >> word;
    if(word.compare(keyword))
    {
        die(keyword, word);
    }

    if(stream.eof())
    {
        die("Parsing error: Attribute corresponding to keyword '" + keyword + "' failed to be found; End-Of-File was reached prematurely");
    }
    stream >> word;
    return word;
}

void YamlReader::die(const std::string& keyword, const std::string& word) const
{
    std::string msg = "graph_analysis::io::YamlReader: Parsing error: keyword '" + keyword + "' expected, but '" + word + "' was found instead";
    LOG_ERROR_S << msg;
    throw std::runtime_error(msg);
}

void YamlReader::die(const std::string& msg) const
{
    std::string msg_stamped = std::string("graph_analysis::io::YamlReader: ") + msg;
    LOG_ERROR_S << msg_stamped;
    throw std::runtime_error(msg_stamped);
}

} // end namespace io
} // end namespace graph_analysis
