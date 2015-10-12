#pragma once

namespace graph_analysis
{

/* This class wraps the VertexIterator to stl (specially foreach compatible)
 * iterators. The Template arguments are:
 * \tparam T Type of the graph_analysis Iterator like graph_analysis::VertexIterator::Ptr
 * \tparam G The Parent class which provides the regular operator like graph_analysis::BaseGraph
 * \tparam E The type the iterator normally returns like graph_analysis::Vertex::Ptr
 */
template <class T, class G, class E>
class BaseIterable
{
   protected:
    typedef T (G::*FuncPtr)() const;

   public:
    /*
     * Entry Point which get's called by the Graph representations.
     * the G *graph is normally a pointer to this from the parent class G
     * the function pointer _func is a pointer to the function of G which
     * construct the graph_library pointer of type T
     */
    BaseIterable(G *graph, FuncPtr _func)
        : graph(graph)
        , func(_func)
    {
    }

    /*
     * Helper class which provides the actual stl-like pointer.
     * It normally only get's constructed by the BaseIterable class
     * and should not be used from somewhere else
     */
    class Iterator
    {
       public:
        Iterator(BaseIterable *parent, T _obj)
            : parent(parent)
            , obj(_obj)
        {
            if (obj.get() && !obj->current()) {
                obj.reset();
            }
        }

        Iterator begin()
        {
            return parent->begin();
        }
        Iterator end()
        {
            return parent->end();
        }

        bool operator==(const Iterator &other) const
        {
            return !operator!=(other);
        }

        bool operator!=(const Iterator &other) const
        {
            return other.obj.get() != obj.get();
        }

        Iterator &operator++()
        {
            if (!obj->next()) {
                obj.reset();
            }
            return *this;
        }

        Iterator &operator++(int)
        {
            Iterator &ret = *this;
            this->operator++();
            return ret;
        }

        E operator*() const
        {
            return obj->current();
        }

       private:
        BaseIterable *parent;
        T obj;
    };

   public:
    /* Created the Interator which points to the first element
     * If no element can be generated (e.G. next() from the baseclass false
     * it returns end() instead
     */
    Iterator begin()
    {
        T it = (graph->*func)();
        if (!it->next()) {
            return end();
        }
        return Iterator(this, it);
    }
    /*
     * This functions returns a Iterator where the Pointer to the
     * shared_pointer T is not-initialized (zero pointer)
     */
    Iterator end()
    {
        return Iterator(this, T());
    }

   protected:
    // Underlayin graph
    G *graph;
    // function pointer which can construct a graph_anaysis pointer of type T
    FuncPtr func;
};

/* This class is child of BaseIterable, it is intended to use in case the construction
 * of a iterator depends on a other object. This can be the case if a iterator for a child-graph or a directed
 * graph needs to be constructed. The addition Type D points to the needed object type.
 */
template <class T, class G, class E, class D>
class SpecializedIterable : public BaseIterable<T, SpecializedIterable<T, G, E, D>, E>
{
   public:
    typedef T (G::*FuncPtr)(const D&) const;

    /*
     * In difference to the BaseIterable class this function needs a pointer to the parent class G, (same as previously) and
     * a Object of type D, which is the needed object to construct a Iterator of this object.
     * The FuncPtr _f2 is the needed function which accepts a D as parameter and constructs a Iterator of type T
     * The parent of BaseIterable does not point to G, instead to this SpecializedIterable<T, G, E, D>
     */
    SpecializedIterable(const G * _parent2, const D& _d, FuncPtr _f2)
        : BaseIterable<T, SpecializedIterable<T, G, E, D>, E>(this, &SpecializedIterable<T, G, E, D>::get)
        , f2(_f2)
        , parent2(_parent2)
        , d(_d)
    {
    }

    /*
     * Helper function which gets used by BaseIterable to construct a new Iterator object.
     * to do so it uses the object D d object, to call the function f2 from G parent2
     */
    T get() const
    {
        return (parent2->*f2)(d);
    };

   protected:
    FuncPtr f2;
    const G *parent2;
    D d;
};
};
