#include "QBaseGraph.hpp"

namespace graph_analysis
{
namespace gui
{

struct null_deleter
{
        void operator()(void const *) const {}
};

QBaseGraph::QBaseGraph(QObject* parent)
    : QObject(parent)
    , mpGraph(graph_analysis::BaseGraph::getInstance())
{
    mpGraph->addObserver( BaseGraphObserver::Ptr(this, null_deleter()) );
}

QBaseGraph::QBaseGraph(graph_analysis::BaseGraph::Ptr graph, QObject* parent)
    : QObject(parent)
    , mpGraph(graph)
{
    mpGraph->addObserver( BaseGraphObserver::Ptr(this, null_deleter()) );
}

QBaseGraph::~QBaseGraph()
{
    mpGraph->removeObserver( BaseGraphObserver::Ptr(this, null_deleter()) );
}

void QBaseGraph::notify(const Vertex::Ptr& vertex, const EventType& event,
                    const GraphId& origin)
{
    emit graphChanged();
}

void QBaseGraph::notify(const Edge::Ptr& edge, const EventType& event,
                    const GraphId& origin)
{
    emit graphChanged();
}

}
}
