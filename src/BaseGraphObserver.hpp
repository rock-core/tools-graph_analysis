#ifndef GRAPH_ANALYSIS_BASE_GRAPH_OBSERVER_HPP
#define GRAPH_ANALYSIS_BASE_GRAPH_OBSERVER_HPP

#include "Edge.hpp"
#include "Vertex.hpp"

namespace graph_analysis
{

/**
 *\brief Types of events for the observer.
 * Example: notify(vertex, EVENT_TYPE_ADDED, ...) is called whenever a new
 *vertex is added to a base graph
 */
enum EventType
{
    EVENT_TYPE_ADDED,
    EVENT_TYPE_REMOVED
};

/**
 * @brief Transaction events
 * These events are triggers for the notification system to allow transactions
 * to be framed. E.g. perform rollbacks of entire transactions, or store
 * transaction events and flush them once a transaction is completed.
 */
enum TransactionType
{
    TRANSACTION_START,
    TRANSACTION_STOP
};

/**
 * \brief Virtual interface class for observing a base graph
 */
class BaseGraphObserver
{
public:
    typedef shared_ptr<BaseGraphObserver> Ptr;

    virtual ~BaseGraphObserver() {}

    virtual void notify(const Vertex::Ptr& vertex, const EventType& event,
                        const GraphId& origin)
    {
        throw std::runtime_error(
            "BaseGraphObserver::notify(Vertex) not implemented");
    };
    virtual void notify(const Edge::Ptr& edge, const EventType& event,
                        const GraphId& origin)
    {
        throw std::runtime_error(
            "BaseGraphObserver::notify(Edge) not implemented");
    };
    virtual void notify( const TransactionType& event, const GraphId& origin )
    {
        // does not need to throw if not implemented
        // this is also to keep backwards compatibility
    };
};
}
#endif
