#include "Player.hpp"
#include "GraphWidget.hpp"
#include "../algorithms/BFS.hpp"
#include <boost/core/null_deleter.hpp>
#include <set>
#include <algorithm>
#include <chrono>
#include <iostream>

namespace graph_analysis {
namespace gui {

PlayerThread::PlayerThread(const std::vector<GraphElement::Ptr>& playlist, uint32_t graphId, const PlayerConfiguration& configuration)
    : mPlaylist(playlist)
    , mGraphId(graphId)
    , mConfiguration(configuration)
{
}

void PlayerThread::run()
{
    bool showItem = mConfiguration.getShowItem();
    size_t delayInMs = mConfiguration.getItemShowtimeDelayInMs();

    setVisibility(mPlaylist, !showItem, 0);

    if(!showItem)
    {
        std::vector<GraphElement::Ptr> playlist = mPlaylist;
        std::reverse(playlist.begin(), playlist.end());
        setVisibility(playlist, showItem, delayInMs);
    } else {
        setVisibility(mPlaylist, showItem, delayInMs);
    }
}

void PlayerThread::setVisibility(const std::vector<GraphElement::Ptr>& elements, bool showItem, size_t delayInMs)
{
    for(const GraphElement::Ptr& ge : elements)
    {
        Vertex::Ptr v = dynamic_pointer_cast<Vertex>(ge);
        if(v)
        {
            emit setVertexVisible(v->getId( mGraphId ), showItem);
            if(delayInMs)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delayInMs));
            }

        } else {
            Edge::Ptr e = dynamic_pointer_cast<Edge>(ge);
            if(e)
            {
                emit setEdgeVisible(e->getId( mGraphId ), showItem);
                if(delayInMs)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(delayInMs));
                }
            }
        }
    }
}

void Player::identifyPlaylist()
{
    graph_analysis::BaseGraph& graph = *mpGraphWidget->mpGraph;
    Vertex::PtrList vertices = graph.getAllVertices();
    for(const Vertex::Ptr& v : vertices)
    {
        if(graph.getInEdges(v).empty())
        {
            mRootVertices.push_back(v);
        }
    }

    // Start BFS for every root node
    for(const Vertex::Ptr& v : mRootVertices)
    {
        appendUnique(mPlayList, v);
        BFSVisitor::Ptr visitor(this, boost::null_deleter());
        algorithms::BFS bfs(mpGraphWidget->mpGraph, visitor);
        bfs.run(v);
    }

    while(true)
    {
        // Rest of graph might contain loops
        // so identify remaining edges
        std::sort(vertices.begin(), vertices.end());
        std::vector<Vertex::Ptr>::iterator it;
        std::vector<Vertex::Ptr> v(vertices.size() - mIdentifiedVertices.size());

        it = std::set_difference(vertices.begin(), vertices.end(),
                mIdentifiedVertices.begin(), mIdentifiedVertices.end(), v.begin());
        v.resize(it - v.begin());

        if(v.empty())
        {
            break;
        } else if(v.size() == 1)
        {
            mIdentifiedVertices.insert(v.front());
            appendUnique(mPlayList, v.front());
        } else {
            // so pick random vertex and try to continue
            Vertex::Ptr startVertex = v.front();

            appendUnique(mPlayList, startVertex);
            BFSVisitor::Ptr visitor(this, boost::null_deleter());
            algorithms::BFS bfs(mpGraphWidget->mpGraph, visitor);
            bfs.run(startVertex);
        }

    }

    std::vector<GraphElement::Ptr> playlist;
    std::vector<GraphElement::Ptr>::iterator vit = mPlayList.begin();
    for(; vit != mPlayList.end(); ++vit)
    {
        Vertex::Ptr currentVertex = dynamic_pointer_cast<Vertex>(*vit);
        if(!currentVertex)
        {
            continue;
        }

        // add in edges first
        for(const GraphElement::Ptr& otherElement : playlist)
        {
            Vertex::Ptr otherVertex = dynamic_pointer_cast<Vertex>(otherElement);
            if(otherVertex)
            {
                std::vector<Edge::Ptr> edges = graph.getEdges(otherVertex, currentVertex);
                playlist.insert(playlist.end(), edges.begin(), edges.end());
            }
        }
        appendUnique(playlist, currentVertex);

        // add out edges /including self edges
        for(const GraphElement::Ptr& otherElement : playlist)
        {
            Vertex::Ptr otherVertex = dynamic_pointer_cast<Vertex>(otherElement);
            if(otherVertex)
            {
                std::vector<Edge::Ptr> edges = graph.getEdges(currentVertex, otherVertex);
                playlist.insert(playlist.end(), edges.begin(), edges.end());
            }
        }
    }
    mPlayList = playlist;
}

void Player::discoverVertex(Vertex::Ptr& vertex)
{
    mIdentifiedVertices.insert(vertex);
    appendUnique(mPlayList, vertex);
}



Player::Player(GraphWidget* graphWidget)
    : BFSVisitor()
    , mpGraphWidget(graphWidget)
    , mPlayerThread(0)
{}

Player::~Player()
{}

void Player::start(PlayerConfiguration configuration)
{
    if(!mpGraphWidget->mpGraph || mpGraphWidget->mpGraph->empty())
    {
        return;
    }

    identifyPlaylist();
    mPlayerThread = new PlayerThread(mPlayList, mpGraphWidget->mpGraph->getId(), configuration);
    connect(mPlayerThread, SIGNAL(setVertexVisible(int,bool)), mpGraphWidget, SLOT(setVertexVisible(int,bool)));
    connect(mPlayerThread, SIGNAL(setEdgeVisible(int,bool)), mpGraphWidget, SLOT(setEdgeVisible(int,bool))) ;
    connect(mPlayerThread, SIGNAL(finished()), mPlayerThread, SLOT(deleteLater()));
    mPlayerThread->start();
}

void Player::appendUnique(GraphElement::PtrList& list, const GraphElement::Ptr& ge)
{
    if(std::find(list.begin(), list.end(), ge) == list.end())
    {
        list.push_back(ge);
    }
}

} // end namespace gui
} // end namespace graph_analysis
