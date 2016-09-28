#ifndef GRAPH_ANALYSIS_TRANSACTION_OBSERVER_HPP__
#define GRAPH_ANALYSIS_TRANSACTION_OBSERVER_HPP__

#include <graph_analysis/BaseGraphObserver.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/shared_ptr.hpp>

namespace graph_analysis
{

struct ObserverEvent
{
    GraphElement::Ptr element;
    GraphId origin;
    EventType event;

    ObserverEvent();
    ObserverEvent( GraphElement::Ptr element, GraphId origin, EventType event );

    /** @brief return true if a match is found between two events
     * @return true if events cancel each other out.  This is e.g. the case
     * when for an add/remove on the same element.
     */
    bool match( const ObserverEvent& other ) const;
};

/**
 * \brief A transaction observer
 *
 * Store all events until a transaction has been ended. 
 * Once the transaction is ended, all events are forwared to the registered
 * observer at once.
 *
 * If no transaction has been started, the events are passed through directly.
 *
 * The transaction events TRANSACTION_START and TRANSACTION_STOP can be nested.
 * Events are then held until the outermost transaction has been finished.
 *
 * \verbatim
 * Usage:
 * 
 * BaseGraphObserver::Ptr obs( new BaseGraphObserver() );
 *
 * TransactionObserver::Ptr tobs = TransactionObserver::getInstance( obs );
 * graph->addObserver( tobs );
 * graph->transactionsEvent( TRANSACTION_START );
 * // modify graph vertices and edges
 * graph->transactionsEvent( TRANSACTION_START );
 * graph->transactionEvent( TRANSACTION_STOP );
 * graph->transactionEvent( TRANSACTION_STOP );
 *
 * // events are emited to obs now
 *
 * \endverbatim
 */
class TransactionObserver : public BaseGraphObserver
{
public:
    typedef shared_ptr<TransactionObserver> Ptr;

    static TransactionObserver::Ptr getInstance( BaseGraphObserver::Ptr &observer );

    /**
     * @param observer - the observer to which the events are forwarded.
     */
    explicit TransactionObserver( BaseGraphObserver::Ptr &observer );
    virtual ~TransactionObserver();

    virtual void notify(const Vertex::Ptr& vertex, const EventType& event,
                        const GraphId& origin);
    virtual void notify(const Edge::Ptr& edge, const EventType& event,
                        const GraphId& origin);
    virtual void notify( const TransactionType& event, const GraphId& origin );

    /** @brief return true if a transaction is currently running
     */
    bool inTransaction();

private:
    void addEvent( ObserverEvent event );
    void emitEvent( ObserverEvent event );

    std::vector<ObserverEvent> mEvents;
    BaseGraphObserver::Ptr mpObserver;
    int mTransactionLevel;

};
}

#endif
