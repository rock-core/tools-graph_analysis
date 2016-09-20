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
class TransactionObserver
{
public:
    typedef shared_ptr<TransactionObserver> Ptr;

    static TransactionObserver::Ptr getInstance( BaseGraphObserver::Ptr &observer )
    {
        return TransactionObserver::Ptr( new TransactionObserver( observer ) );
    }

    explicit TransactionObserver( BaseGraphObserver::Ptr &observer )
        : observer( observer ) {} 

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
                Vertex::Ptr vertex = dynamic_pointer_cast<Vertex>( it->element );
                if( vertex )
                {
                    observer->notify( vertex, it->event, it->origin );
                    continue;
                }
                Edge::Ptr edge = dynamic_pointer_cast<Edge>( it->element );
                if( edge )
                {
                    observer->notify( edge, it->event, it->origin );
                    continue;
                }
            }

        }
        // in any case, the event queue should be cleared
        events.clear();

        // also forwared the transaction event itself
        observer->notify( event, origin );
    };

private:
    void addEvent( ObserverEvent event )
    {
        // TODO we need to filter here, that paired add/remove events 
        // result in removal of both events alltogether.

        events.push_back( event );
    }

    std::vector<ObserverEvent> events;
    BaseGraphObserver::Ptr observer;

};
}

#endif
