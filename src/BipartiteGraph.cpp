#include <graph_analysis/BipartiteGraph.hpp>

#include <graph_analysis/Edge.hpp>

namespace graph_analysis {

BipartiteGraph::BipartiteGraph()
{
    mpGraph = BaseGraph::getInstance();

    mpRed = BaseGraph::getSubGraph(mpGraph);
    mpBlue = BaseGraph::getSubGraph(mpGraph);
}

void BipartiteGraph::linkVertices(const Vertex::Ptr& red, const Vertex::Ptr& blue, const std::string& label)
{
    try {
        mpGraph->addVertex(red);
    } catch(const std::runtime_error& e)
    {}
    mpRed->enable(red);

    try {
        mpGraph->addVertex(blue);
    } catch(const std::runtime_error& e)
    {}
    mpBlue->enable(blue);

    Edge::Ptr red2Blue(new Edge(red, blue, label));
    Edge::Ptr blue2Red(new Edge(blue, red, label));

    mpGraph->addEdge(red2Blue);
    mpGraph->addEdge(blue2Red);
}


std::vector<Vertex::Ptr> BipartiteGraph::getPartners(const Vertex::Ptr& vertex) const
{
    std::vector<Vertex::Ptr> neighbours;
    EdgeIterator::Ptr eit = mpGraph->getOutEdgeIterator(vertex);
    while(eit->next())
    {
        Edge::Ptr edge = eit->current();
        if(edge->getSourceVertex() != vertex)
        {
            neighbours.push_back(edge->getSourceVertex());
        } else {
            neighbours.push_back(edge->getTargetVertex());
        }
    }
    return neighbours;
}

Vertex::Ptr BipartiteGraph::getUniquePartner(const Vertex::Ptr& vertex) const
{
    std::vector<Vertex::Ptr> neighbours = getPartners(vertex);
    if(neighbours.size() != 1)
    {
        throw std::invalid_argument("graph_analysis::BipartiteGraph::getUniquePartner: vertex '" +
                vertex->toString() + "' has no unique partner");
    }

    return neighbours.front();
}

} // end namespace graph_analysis
