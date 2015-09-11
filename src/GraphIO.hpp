#ifndef GRAPH_ANALYSIS_IO_HPP
#define GRAPH_ANALYSIS_IO_HPP

#include <stdexcept>
#include <graph_analysis/Graph.hpp>

namespace graph_analysis {

namespace representation {

enum Type { UNKNOWN = 0, GEXF, LEMON, YAML, GRAPHVIZ, END_MARKER };

typedef std::string Suffix;

extern std::map<Type, std::string> TypeTxt;

} // end namespace representation

namespace io {

/// GEXF reader/writer common convetions
const std::string CLASS("0");
const std::string LABEL("1");

class Writer
{
public:
    typedef boost::shared_ptr<Writer> Ptr;

    virtual ~Writer() {}

    void write(const std::string& filename, const BaseGraph& graph) const;

    virtual void write(const std::string& filename, const BaseGraph::Ptr& graph) const { throw std::runtime_error("Writer: writer not implemented"); }
};


class Reader
{
public:
    typedef boost::shared_ptr<Reader> Ptr;

    virtual ~Reader() {}

    void read(const std::string& filename, BaseGraph& graph);

    virtual void read(const std::string& filename, BaseGraph::Ptr graph) { throw std::runtime_error("Reader: reader not implemented"); }
};


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
