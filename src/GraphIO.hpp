#ifndef GRAPH_ANALYSIS_IO_HPP
#define GRAPH_ANALYSIS_IO_HPP

#include <stdexcept>
#include <graph_analysis/Graph.hpp>

namespace graph_analysis {

namespace representation {

enum Type { UNKNOWN = 0, GEXF, LEMON, YAML, GRAPHVIZ, OROGEN_MODEL, END_MARKER };

typedef std::string Suffix;

extern std::map<Type, std::string> TypeTxt;

} // end namespace representation

namespace io {

/// GEXF reader/writer common conventions
const std::string CLASS("0");
const std::string LABEL("1");

/**
 * \class Writer
 * \brief Interface for Writer implementations that perform custom
 * graph serialization
 */
class Writer
{
public:
    typedef shared_ptr<Writer> Ptr;

    virtual ~Writer() {}

    /**
     * Write graph to a file
     * \param filename Filename to write to
     * \param graph
     */
    void write(const std::string& filename, const BaseGraph& graph) const;

    /**
     * Subclasses have to implement this function in order to provide
     * writing capability
     */
    virtual void write(const std::string& filename, const BaseGraph::Ptr& graph) const { (void)filename; (void)graph; throw std::runtime_error("Writer: writer not implemented"); }
};

/**
 * \class Reader
 * \brief Interface for Reader implementation that perform custom
 * graph deserialization
 */
class Reader
{
public:
    typedef shared_ptr<Reader> Ptr;

    virtual ~Reader() {}

    void read(const std::string& filename, BaseGraph& graph);

    virtual void read(const std::string& filename, BaseGraph::Ptr graph) { (void) filename; (void) graph; throw std::runtime_error("Reader: reader not implemented"); }
};

/**
 * \class GraphIO
 * \brief Main class for handling I/O for graphs
 * \details The write/read functions try
 * to infer the graph format from the filename suffix
 * if no format is specified.
 *
 * Example:
 \verbatim
 #include<graph_analysis/GraphIO.hpp>
 ...
 BaseGraph::Ptr graph = BaseGraph::getInstance();
 ...
 io::GraphIO::write("test-file.gexf", graph);
 io::GraphIO::write("test-file.dot", graph);
 \endverbatim
 *
 *
 * In order to allow custom vertices and edges to be serialized and deserialized
 * you need to properly overload the getClassName and getClone methods (as you
 * should always do)
 *
 \verbatim
 class MyVertex : Vertex
 {
 public:
    virtual std::string getClassName() const { return "MyVertex"; }

 protected:
    virtual Vertex* getClone() { return new MyVertex(*this); }
 };

 class MyEdge : Edge
 {
 public:
    virtual std::string getClassName() const { return "MyEdge"; }
 protected:
    virtual Edge* getClone() { return new MyEdge(*this); }
 };

 \endverbatim

 *
 * Register your custom vertex and edge types, e.g., at the beginning of your program or
 * in the constructor of a custom graph type that used the edges and vertices
 *
 * TODO: improve registration using singleton like initialization
 *
 \verbatim
 try {
     VertexTypeManager* vertexTypeManager = VertexTypeManager::getInstance();
     vertexTypeManager->registerType("MyVertex", Vertex::Ptr(new MyVertex()), true);

     EdgeTypeManager* edgeTypeManager = EdgeTypeManager::getInstance();
     edgeTypeManager->registerType("MyEdge", Edge::Ptr(new MyEdge()), true);
 } catch(...)
 {
     // already registered
 }
 \endverbatim
 *
 * Eventually, the reader has to support/take care of proper edge construction and vertex
 * construction, e.g., in GexfReader
 \verbatim
    libgexf::t_id current = edge_it->next();
    std::string edgeClass = data.getEdgeAttribute(current, classAttr);
    std::string edgeLabel = data.getEdgeAttribute(current, labelAttr);
    Vertex::Ptr sourceVertex = vertexMap[edge_it->currentSource()]; // NOTE: assumes the .gexf(.xml) file is valid
    Vertex::Ptr targetVertex = vertexMap[edge_it->currentTarget()]; // NOTE: assumes the .gexf(.xml) file is valid
    Edge::Ptr edge = EdgeTypeManager::getInstance()->createEdge(edgeClass, sourceVertex, targetVertex, edgeLabel);
    graph->addEdge(edge);
 \endverbatim

 * To fully support (de)serialization of a graph, edges and vertices have to be
 * store custom attribute data -- apart from the default 'class' and 'label'
 * attributes.
 * For that purpose a general mechanism exists in order to implement a custom
 * serialization:
 *
 * \verbatim
class DerivedVertex : public graph_analysis::Vertex
{
public:
    DerivedVertex(std::string name)
        : graph_analysis::Vertex(name)
    {}

    std::string serializeMember0() { return mMember0; }
    std::string serializeMember1() { return mMember1; }

    void deserializeMember0(const std::string& s) { mMember0= s; }
    void deserializeMember1(const std::string& s) { mMember1 = s; }

    virtual std::string getClassName() const{ return "DerivedVertex"; }

    std::string mMember0;
    std::string mMember1;
protected:
    virtual graph_analysis::Vertex* getClone() const { return new DerivedVertex("CLONE"); }

};

class DerivedEdge : public graph_analysis::Edge
{
public:
    DerivedEdge(std::string name)
        : graph_analysis::Edge(name)
    {}

    std::string serializeMember0() { return mMember0; }
    std::string serializeMember1() { return mMember1; }

    void deserializeMember0(const std::string& s) { mMember0= s; }
    void deserializeMember1(const std::string& s) { mMember1 = s; }

    virtual std::string getClassName() const{ return "DerivedEdge"; }

    std::string mMember0;
    std::string mMember1;
protected:
    virtual graph_analysis::Edge* getClone() const { return new DerivedEdge("CLONE"); }

};

    Vertex::Ptr empty( new DerivedVertex("derived-vertex"));
    VertexTypeManager *vManager = VertexTypeManager::getInstance();

    vManager->registerAttribute(empty->getClassName(), "m0",
            (io::AttributeSerializationCallbacks::serialize_func_t)&DerivedVertex::serializeMember0,
            (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedVertex::deserializeMember0,
            (io::AttributeSerializationCallbacks::print_func_t)&DerivedVertex::serializeMember0);

    vManager->registerAttribute(empty->getClassName(), "m1",
            (io::AttributeSerializationCallbacks::serialize_func_t)&DerivedVertex::serializeMember1,
            (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedVertex::deserializeMember1,
            (io::AttributeSerializationCallbacks::print_func_t) &DerivedVertex::serializeMember1);

    Edge::Ptr e0(new DerivedEdge("derived-edge"));
    EdgeTypeManager* eManager = EdgeTypeManager::getInstance();
    eManager->registerType(e0);
    eManager->registerAttribute(e0->getClassName(), "a0",
            (io::AttributeSerializationCallbacks::serialize_func_t) &DerivedEdge::serializeMember0,
            (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedEdge::deserializeMember0,
            (io::AttributeSerializationCallbacks::print_func_t)&DerivedEdge::serializeMember0);

    eManager->registerAttribute(e0->getClassName(), "a1",
            (io::AttributeSerializationCallbacks::serialize_func_t) &DerivedEdge::serializeMember1,
            (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedEdge::deserializeMember1,
            (io::AttributeSerializationCallbacks::print_func_t)&DerivedEdge::serializeMember1);


...

    io::GraphIO::write(filename, graph, representation::GEXF);
    ...
    BaseGraph::Ptr read_graph = BaseGraph::getInstance();
    io::GraphIO::read(filename, read_graph, representation::GEXF);

 \endverbatim
 *
 *
 */
class GraphIO
{
public:
    typedef std::map<representation::Type, Writer::Ptr> WriterMap;
    typedef std::map<representation::Type, Reader::Ptr> ReaderMap;
    typedef std::map<representation::Suffix, representation::Type> SuffixMap;

    static void write(const std::string& filename, const BaseGraph::Ptr& graph, representation::Type format = representation::UNKNOWN);
    static void write(const std::string& filename, const BaseGraph& graph, representation::Type format = representation::UNKNOWN);

    static void read(const std::string& filename, BaseGraph::Ptr graph, representation::Type format = representation::UNKNOWN);
    static void read(const std::string& filename, BaseGraph& graph, representation::Type format = representation::UNKNOWN);

    static WriterMap getWriterMap() { return msWriters; }
    static ReaderMap getReaderMap() { return msReaders; }
    static SuffixMap getSuffixMap() { return msSuffixes; }

protected:
    static representation::Type getTypeFromSuffix(representation::Suffix suffix);
    static representation::Type getTypeFromFilename(const std::string& filename);
    static std::string appendSuffix(const std::string& filename, representation::Type format);
    static representation::Suffix getSuffix(representation::Type format);

private:
    static WriterMap msWriters;
    static ReaderMap msReaders;
    static SuffixMap msSuffixes;
};

} // end namespace io
} // end namespace graph_analysis

#endif //GRAPH_ANALYSIS_IO_HPP
