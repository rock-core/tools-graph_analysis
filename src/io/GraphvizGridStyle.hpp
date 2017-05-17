#ifndef GRAPH_ANALYSIS_IO_GRAPHVIZ_GRID_STYLE_HPP
#define GRAPH_ANALYSIS_IO_GRAPHVIZ_GRID_STYLE_HPP

#include "GraphvizStyle.hpp"
#include "../SharedPtr.hpp"

namespace graph_analysis {
namespace io {

typedef function1<size_t, const Vertex::Ptr& > VertexRowMapper;
typedef function1<size_t, const Vertex::Ptr& > VertexColumnMapper;

/**
 * \class GraphvizGridStyle
 * \brief Allows the alignment of nodes into a grid, i.e. perform some kind of
 * prealignment using the pos attribute
 * However, this will only work when: (a) using the 'canon' renderer and (b)
 * using fdp for later layouting.
 * For example:
 *
 \verbatim
class RowColumnVertex : public graph_analysis::Vertex
{
public:
    typedef shared_ptr<RowColumnVertex> Ptr;

    RowColumnVertex(const std::string& name, size_t row, size_t column)
        : graph_analysis::Vertex(name)
        , row(row)
        , column(column)
    {}

    virtual std::string getClassName() const{ return "RowColumnVertex"; }

    size_t row;
    size_t column;

    static size_t getRow(const graph_analysis::Vertex::Ptr& vertex)
    {
        return dynamic_pointer_cast<RowColumnVertex>(vertex)->row;
    }

    static size_t getColumn(const graph_analysis::Vertex::Ptr& vertex)
    {
        return dynamic_pointer_cast<RowColumnVertex>(vertex)->column;
    }

protected:
    virtual graph_analysis::Vertex* getClone() const { return new RowColumnVertex(getLabel(), row, column); }

};
    io::GraphvizWriter gvWriter("dot","canon");
    io::GraphvizGridStyle::Ptr style(new io::GraphvizGridStyle(3,3,
                &RowColumnVertex::getRow,
                &RowColumnVertex::getColumn
                ));
    style->setColumnScalingFactor(2.0);
    style->setRowScalingFactor(2.0);
    gvWriter.setStyle(style);

    gvWriter.write("graphviz_grid_layout.dot", graph);
 \endverbatim
 * Eventually render with:
 \verbatim
 xdot -f fdp graphviz_grid_layout.dot
 \endverbatim
 or
 \verbatim
 dot -s -Kfdp -Tpng graphviz_grid_layout.dot > layouted_grid.png
 \endverbatim
 *
 *
 * TODO: alternatively one could use an approach using rankdir and {rank=same;
 * ...}
 *
 */
class GraphvizGridStyle : public GraphvizStyle
{
public:
    GraphvizGridStyle(size_t rows, size_t columns,
            VertexRowMapper rowMapperFunction,
            VertexColumnMapper columnMapperFunction
            );

    virtual ~GraphvizGridStyle();

    typedef shared_ptr<GraphvizGridStyle> Ptr;

    void setColumnScalingFactor(double f) { mColumnScalingFactor = f; }
    double getColumnScalingFactor() const { return mColumnScalingFactor; }

    void setRowScalingFactor(double f) { mRowScalingFactor = f; }
    double getRowScalingFactor() const { return mRowScalingFactor; }

    virtual void apply(const Edge::Ptr& edge, GVGraph* gvGraph, const BaseGraph::Ptr& graph) const {}
    virtual void apply(const Vertex::Ptr& vertex, GVGraph* gvGraph, const BaseGraph::Ptr& graph) const;

private:
    size_t mRows;
    size_t mColumns;

    double mRowScalingFactor;
    double mColumnScalingFactor;

    VertexRowMapper mRowMapperFunction;
    VertexColumnMapper mColumnMapperFunction;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GRAPHVIZ_GRID_STYLE_HPP
