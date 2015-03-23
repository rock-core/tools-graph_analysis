#include "FloydWarshall.hpp"

namespace graph_analysis {
namespace algorithms {

double FloydWarshall::getDistanceLazy(DistanceMatrix& distanceMatrix, Vertex::Ptr i, Vertex::Ptr k)
{
    DistanceMatrix::iterator itDistanceIK = distanceMatrix.find(std::pair<Vertex::Ptr, Vertex::Ptr>(i,k));
    // Lazy initialization of pair
    double distanceIK = std::numeric_limits<double>::infinity();
    if(itDistanceIK != distanceMatrix.end())
    {
        distanceIK = itDistanceIK->second;
    } else {
        distanceMatrix[std::pair<Vertex::Ptr, Vertex::Ptr>(i,k)] = distanceIK;
    }

    return distanceIK;
}

DistanceMatrix FloydWarshall::allShortestPaths(BaseGraph::Ptr graph, EdgeWeightFunction edgeWeightFunction, bool detectNegativeCycle)
{
    DistanceMatrix distanceMatrix;

    // Initialize the distance matrix
    VertexIterator::Ptr vertexIt = graph->getVertexIterator();
    while(vertexIt->next())
    {
        Vertex::Ptr vertex = vertexIt->current();
        distanceMatrix[ std::pair<Vertex::Ptr, Vertex::Ptr>(vertex,vertex) ] = 0;
    }

    EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        distanceMatrix[ std::pair<Vertex::Ptr, Vertex::Ptr>(edge->getSourceVertex(), edge->getTargetVertex()) ] = edgeWeightFunction(edge);
    }

    VertexIterator::Ptr i_vertexIt = graph->getVertexIterator();
    while(i_vertexIt->next())
    {
        Vertex::Ptr i = i_vertexIt->current();

        VertexIterator::Ptr j_vertexIt = graph->getVertexIterator();
        while(j_vertexIt->next())
        {
            Vertex::Ptr j = j_vertexIt->current();
            double distanceJI = getDistanceLazy(distanceMatrix,j,i);

            VertexIterator::Ptr k_vertexIt = graph->getVertexIterator();
            while(k_vertexIt->next())
            {
                Vertex::Ptr k = k_vertexIt->current();
                double distanceJK = getDistanceLazy(distanceMatrix,j,k);
                double distanceIK = getDistanceLazy(distanceMatrix,i,k);
                double transitiveDistanceJK = distanceJI + distanceIK;

                if( distanceJK > transitiveDistanceJK)
                {
                    if(detectNegativeCycle && j == k && transitiveDistanceJK < 0)
                    {
                        throw std::runtime_error("graph_analysis::algorithms::FloydWarshall::allShortestPaths: negative cycle detected");
                    } else {
                        distanceMatrix[ std::pair<Vertex::Ptr, Vertex::Ptr>(j,k) ] = transitiveDistanceJK;
                    }
                }
            }
        }
    }
    return distanceMatrix;
}
} // end namespace algorithms
} // end namespace graph_analysis
