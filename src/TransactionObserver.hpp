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

    ObserverEvent() {} 
    ObserverEvent( GraphElement::Ptr element, GraphId origin, EventType event )
        : element( element ), origin( origin ), event( event ) {}

    /** @brief return true if a match is found between two events
     * @return true if events cancel each other out.  This is e.g. the case
     * when for an add/remove on the same element.
     */
    bool match( const ObserverEvent& other ) const
    {
        if( element == other.element && origin == other.origin )
        {
            if( (event == EVENT_TYPE_ADDED && other.event == EVENT_TYPE_REMOVED) ||
                (event == EVENT_TYPE_REMOVED && other.event == EVENT_TYPE_ADDED) ) 
            {
                return true;
            }

            if( event == other.event )
                throw std::runtime_error("Event type for same elementId has been called twice. This should not happen."); 

        }
        return false;
    }
};

/**
 * \brief A transaction observer
 *
 * Store all events until a transaction has been ended. 
 * Once the transaction is ended, all events are forwared to the registered
 * observer at once.
 *
 */
class TransactionObserver : public BaseGraphObserver
{
public:
    typedef shared_ptr<TransactionObserver> Ptr;

    static TransactionObserver::Ptr getInstance( BaseGraphObserver::Ptr &observer )
    {
        return TransactionObserver::Ptr( new TransactionObserver( observer ) );
    }

    explicit TransactionObserver( BaseGraphObserver::Ptr &observer )
        : observer( observer ), transactionLevel( 0 ) {} 

    virtual ~TransactionObserver() {}

    virtual void notify(const Vertex::Ptr& vertex, const EventType& event,
                        const GraphId& origin)
    {
        addEvent( ObserverEvent( vertex, origin, event ) );
    };
    virtual void notify(const Edge::Ptr& edge, const EventType& event,
                        const GraphId& origin)
    {
        addEvent( ObserverEvent( edge, origin, event ) );
    };
    virtual void notify( const TransactionType& event, const GraphId& origin )
    {
        // for a TRANSACTION_STOP event, we will emit all the events collected so far
        if( event == TRANSACTION_STOP )
        {
            for( std::vector<ObserverEvent>::iterator it = events.begin(); it != events.end(); ++it )
            {
                emitEvent( *it );
            }
            events.clear();
            transactionLevel -= 1;
            
            if( transactionLevel < 0 )
                throw std::runtime_error("Got TRANSACTION_STOP event without a corresponding start.");
        }

        if( event == TRANSACTION_START )
        {
            transactionLevel += 1;
        }

        // also forwared the transaction event itself
        observer->notify( event, origin );
    };

    /** @brief return true if a transaction is currently running
     */
    bool inTransaction()
    {
        return transactionLevel > 0;
    }

private:
    void addEvent( ObserverEvent event )
    {
        // TODO we need to filter here, that paired add/remove events 
        // result in removal of both events alltogether.

        // only hold events when in a transaction
        if( inTransaction() )
            events.push_back( event );
        else
            emitEvent( event );
    }

    void emitEvent( ObserverEvent event )
    {
        Vertex::Ptr vertex = dynamic_pointer_cast<Vertex>( event.element );
        if( vertex )
        {
            observer->notify( vertex, event.event, event.origin );
            return; 
        }
        Edge::Ptr edge = dynamic_pointer_cast<Edge>( event.element );
        if( edge )
        {
            observer->notify( edge, event.event, event.origin );
            return;
        }
    }

    std::vector<ObserverEvent> events;
    BaseGraphObserver::Ptr observer;
    int transactionLevel;

};
}

#endif
