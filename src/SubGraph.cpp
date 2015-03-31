#include "SubGraph.hpp"

#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>

namespace graph_analysis {

SubGraph::SubGraph(BaseGraph* baseGraph)
    : mpBaseGraph(baseGraph)
{}

BaseGraph* SubGraph::getBaseGraph()
{ 
    return mpBaseGraph;
}

/**
 * Apply filters to this subgraph
 */
void SubGraph::applyFilters(Filter<Vertex::Ptr>::Ptr vertexFilter, Filter<Edge::Ptr>::Ptr edgeFilter)
{
    if(edgeFilter)
    {
        filters::EdgeContextFilter::Ptr contextFilter = boost::dynamic_pointer_cast<filters::EdgeContextFilter>(edgeFilter);

        EdgeIterator::Ptr edgeIterator = getBaseGraph()->getEdgeIterator();

        while(edgeIterator->next())
        {
            // By default edges are disabled
            Edge::Ptr edge = edgeIterator->current();
            if( edgeFilter->permits(edge) )
            {
                // A context filter should apply to source / target nodes -- no need to filter this edge specifically then
                if(contextFilter)
                {
                    bool filterTarget = contextFilter->permitsTarget(edge);
                    bool filterSource = contextFilter->permitsSource(edge);

                    if(filterSource && filterTarget)
                    {
                        enable(edge);
                    }
                }
            }
        }
    }

    if(vertexFilter)
    {
        VertexIterator::Ptr vertexIterator = getBaseGraph()->getVertexIterator();
        while(vertexIterator->next())
        {
            Vertex::Ptr vertex = vertexIterator->current();
            if( vertexFilter->permits(vertex) )
            {
                enable(vertex);
            }
        }
    }
}

} // end namespace graph_analysis
