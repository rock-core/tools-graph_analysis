#include "YamlReader.hpp"
#include <base/Logging.hpp>
#define IS_EMPTY(X) (0 == X.rdbuf()->in_avail())

namespace graph_analysis {
namespace io {

void YamlReader::read(const std::string& filename, const BaseGraph::Ptr& graph) const
{
    const char *fname = filename.c_str();
    std::ifstream fin(fname);
    if(!fin.is_open() || fin.fail() || fin.eof())
    {
        std::string error_msg = std::string("graph_analysis::io::YamlReader: failed to open input file ") + filename + " for graph import  OR  the same input file was empty";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    LOG_INFO("graph_analysis::io::YamlWriter: importing graph from file \"%s\" ", fname);


    graph->clear();
    VertexMap vMap;
    std::string line;
    std::getline(fin, line);
    std::stringstream nodeStream, edgeStream;
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
    parseNodes(nodeStream, graph, vMap);
    parseEdges(edgeStream, graph, vMap);
}


void YamlReader::parseNodes(std::stringstream& nodeStream, const BaseGraph::Ptr& graph, VertexMap& vMap) const
{
    char c;
    std::string idWord, labelWord, typeWord;
    while(!nodeStream.eof())
    {
        do
        {
            nodeStream >> c;
        }
        while('-' != c && !nodeStream.eof());

        idWord = nextToken("id:", nodeStream, graph);
        labelWord = nextToken("label:", nodeStream, graph);
        typeWord = nextToken("type:", nodeStream, graph); // TODO: currently discards type issues (useful for a to-be-implemented feature of node clustering/grouping)
        Vertex::Ptr vertex(new Vertex(labelWord));
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
        do
        {
            edgeStream >> c;
        }
        while('-' != c && !edgeStream.eof());

        fromNodeWord = nextToken("fromNodeId:", edgeStream, graph);
        toNodeWord = nextToken("toNodeId:", edgeStream, graph);
        labelWord = nextToken("label:", edgeStream, graph);
        Vertex::Ptr sourceVertex = vMap[fromNodeWord]; // NOTE: assumes the .yml (.yaml) file is valid
        Vertex::Ptr targetvertex = vMap[toNodeWord]; // NOTE: assumes the .yml (.yaml) file is valid
        Edge::Ptr edge(new Edge(sourceVertex, targetvertex, labelWord));
        graph->addEdge(edge);
    }
}

std::string YamlReader::nextToken(const std::string& keyword, std::stringstream& stream, const BaseGraph::Ptr& graph) const
{
    if(IS_EMPTY(stream))
    {
        die("Parsing error: keyword '" + keyword + "' failed to be found; End-Of-File was reached prematurely");
    }

    std::string word;
    stream >> word;
    if(word.compare(keyword))
    {
        die(keyword, word);
    }

    if(IS_EMPTY(stream))
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
