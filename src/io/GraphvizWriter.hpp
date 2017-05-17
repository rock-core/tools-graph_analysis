#ifndef GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP
#define GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP

#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/io/GraphvizStyle.hpp>

namespace graph_analysis {
namespace io {

/**
 * \file GraphvizWriter.hpp
 * \class GraphvizWriter
 * \brief graphviz based graph renderer
 * \details Exports a given base graph to a given dot file using GraphViz renderToFile() interface
 * Vertex and Edges are rendered using their toString() methods
 * For details check GVGraph::getUniqueName
 *
 * In order to apply a certain style to the graph output, the GraphvizWriter
 * provides a GraphvizStyle to implement some hooks
 \verbatim
    #include <graph_analysis/io/GVGraph.hpp>

    using namespace graph_analysis;

    class SimpleStyle : public io::GraphvizStyle
    {
    public:
        void apply(const Edge::Ptr& edge, io::GVGraph* gvGraph, const BaseGraph::Ptr& graph) const
        {
            gvGraph->setAttribute(edge, "color", "red");
            gvGraph->setAttribute(edge, "weight", "10");
            gvGraph->setAttribute(edge, "arrowsize", "2");
            gvGraph->setAttribute(edge, "penwidth", "3");
        }

        void apply(const Vertex::Ptr& vertex, io::GVGraph* gvGraph, const BaseGraph::Ptr& graph) const
        {
            gvGraph->setAttribute(vertex, "color", "blue");
            gvGraph->setAttribute(vertex, "shape", "diamond");
            gvGraph->setAttribute(vertex, "penwidth", "3");
        }
    };
 \endverbatim

 You need to register the style with the GraphvizWriter before writing. Note, that the default writer
 used by GraphIO::write will not have any style associated.
 \verbatim
     BaseGraph::Ptr graph = BaseGraph::getInstance();

     Vertex::Ptr v0(new Vertex("v0"));
     Vertex::Ptr v1(new Vertex("v1"));
     Vertex::Ptr v2(new Vertex("v2"));

     Edge::Ptr e0(new Edge(v0,v1,"e0"));
     Edge::Ptr e1(new Edge(v1,v2,"e1"));
     Edge::Ptr e2(new Edge(v2,v0,"e2"));

     graph->addEdge(e0);
     graph->addEdge(e1);
     graph->addEdge(e2);

     io::GraphvizWriter gvWriter;
     io::GraphvizStyle::Ptr style(new SimpleStyle());
     gvWriter.setStyle(style);

     gvWriter.write("styled-graph.dot",graph);
 \endverbatim
 *
 */
class GraphvizWriter : public Writer
{
    /// layouting engine to re-arrange the graph through GraphViz rendering
    std::string mLayout;
    /// Renderer e.g. dot, pdf, svg, png
    std::string mRenderer;
    GraphvizStyle::Ptr mpStyle;

public:
    /// constructor
    GraphvizWriter(const std::string& layout = "dot", const std::string& renderer = "dot");
    /// destructor
    ~GraphvizWriter();

    /**
     * Allow to set a style sheet
     */
    void setStyle(const GraphvizStyle::Ptr& style) { mpStyle = style; }

    /**
     * \brief outputs the given graph to the given file
     * \param filename requested output filename
     * \param graph requested graph to be printed
     */
    void write(const std::string& filename, const BaseGraph& graph) const;

    /**
     * \brief outputs the given graph to the given file
     * \param filename requested output filename
     * \param graph smart pointer to the requested graph to be printed
     */
    void write(const std::string& filename, const BaseGraph::Ptr& graph) const;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP
