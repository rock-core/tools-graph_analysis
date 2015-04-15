#ifndef GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP
#define GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP

#include <QGraphicsView>
#include "../gui/EdgeItem.hpp"
#include "../gui/NodeItem.hpp"
#include "../gui/NodeTypeManager.hpp"
#include "../gui/EdgeTypeManager.hpp"

#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/gui/GVGraph.hpp>

namespace graph_analysis {
namespace io {

class GraphvizWriter : public Writer
{
    
    typedef std::map<graph_analysis::Vertex::Ptr, graph_analysis::gui::NodeItem*> NodeItemMap;
    // Allow mapping from graph vertexes to nodes in the scene
    NodeItemMap mNodeItemMap;
    graph_analysis::gui::GVGraph* mpGVGraph;
    std::string mLayout;
public:
    GraphvizWriter(const std::string &layout = "dot");
    ~GraphvizWriter();
    void write(const std::string& filename, BaseGraph* graph);
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP