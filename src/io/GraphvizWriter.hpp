#ifndef GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP
#define GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP

#include <QGraphicsView>
#include "../gui/EdgeItem.hpp"
#include "../gui/NodeItem.hpp"
#include "../gui/NodeTypeManager.hpp"
#include "../gui/EdgeTypeManager.hpp"

#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/gui/GVGraph.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis {
namespace io {

class GraphvizWriter : public Writer
{
    std::string mLayout;
public:
    GraphvizWriter(const std::string &layout = "dot");
    ~GraphvizWriter();
    void write(const std::string& filename, const BaseGraph& graph) const;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP
