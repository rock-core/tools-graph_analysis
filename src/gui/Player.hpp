#ifndef GRAPH_ANALYSIS_GUI_PLAYER_HPP
#define GRAPH_ANALYSIS_GUI_PLAYER_HPP

#include "../algorithms/BFSVisitor.hpp"
#include "EdgeItemBase.hpp"
#include "VertexItemBase.hpp"

#include "PlayerConfiguration.hpp"
#include <QObject>
#include <QThread>

namespace graph_analysis {
namespace gui {

class GraphWidget;

class PlayerThread : public QThread
{
    Q_OBJECT

public:
    PlayerThread(const std::vector<GraphElement::Ptr>& playlist, uint32_t graphId, const PlayerConfiguration& configuration);
    void run();

    void setVisibility(const std::vector<GraphElement::Ptr>& elements,
            bool showItem,
            size_t delayInMs);


signals:
    void setEdgeVisible(int,bool);
    void setVertexVisible(int,bool);

private:
    std::vector<GraphElement::Ptr> mPlaylist;
    uint32_t mGraphId;
    PlayerConfiguration mConfiguration;
};

/**
 * Class inorder to incrementally build up the graph displayed
 * in the graph widget
 */
class Player : QObject, public graph_analysis::algorithms::BFSVisitor
{
    Q_OBJECT

public:
    Player(GraphWidget* graphWidget);
    virtual ~Player();

    void start(PlayerConfiguration configuration = PlayerConfiguration());

    void identifyPlaylist();

    void discoverVertex(Vertex::Ptr& vertex);

private:
    GraphWidget* mpGraphWidget;

    // Collects the vertices and edges in the order of
    // discovery
    // this might contain duplicate entries
    std::vector<GraphElement::Ptr> mPlayList;
    // Set of root vertices
    Vertex::PtrList mRootVertices;
    // Set of identified vertices to allow
    // handling of cycles, etc
    Vertex::PtrSet mIdentifiedVertices;

    PlayerThread* mPlayerThread;

    /**
     * Append to playlist if not already part of the playlist
     */
    void appendUnique(std::vector<GraphElement::Ptr>& list, const GraphElement::Ptr& ge);

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_PLAYER_HPP
