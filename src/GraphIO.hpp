#ifndef GRAPH_ANALYSIS_IO_HPP
#define GRAPH_ANALYSIS_IO_HPP

#include <stdexcept>
#include <graph_analysis/Graph.hpp>

namespace graph_analysis {

namespace representation {

enum Type { UNKNOWN = 0, GEXF, LEMON, GRAPHVIZ, END_MARKER };

extern std::map<Type, std::string> TypeTxt;

} // end namespace representation

namespace io {

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

    static void write(const std::string& filename, const BaseGraph& graph, representation::Type format);

    static void read(const std::string& filename, BaseGraph& graph, representation::Type format);

private:
    static WriterMap msWriters;
    static ReaderMap msReaders;
};

} // end namespace io
} // end namespace graph_analysis

#endif //GRAPH_ANALYSIS_IO_HPP
